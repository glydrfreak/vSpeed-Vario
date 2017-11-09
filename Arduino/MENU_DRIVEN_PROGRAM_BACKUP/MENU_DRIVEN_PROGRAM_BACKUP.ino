#include "DEFAULT_SETTINGS.h"
/*#include "MENU.h"*/
#include "BUTTON.h"
#include "OLED.h"

#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define BUTTON_PIN                    A1
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

BUTTON BUTTON;
Default SETTING;
/*MENU THE_MENU;*/
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);


//POSSIBLE ACTIVITY NAMES:
#define NO_ACTIVITY        0
#define MAIN_ACTIVITY      0
#define SETTINGS           1
#define BEEP               2
#define VOL                3
#define THRESHOLD          4
#define CLIMB              5
#define SINK               6
#define PITCH              7
#define CLIMB_MAX          8
#define CLIMB_MIN          9
#define SINK_MAX          10
#define SINK_MIN          11
#define BLUETOOTH         12
#define OLED              13
#define CHART_SPEED       14
#define USER              15
#define EDIT_NAMES        16
#define USER_1            17
#define USER_2            18
#define USER_3            19

//POSSIBLE ACTIONS:
#define NOn_ACTION         0
#define INCREMENT         1
#define DECREMENT         2
#define TOGGLE            3
#define CHANGE_ACTIVITY   4

//FILL THESE ARRAYS FOR ORGANIZATION:
#define numberOfItems                   7  //ITEM_1, ITEM_2, ITEM_3, ITEM_4, ITEM5, GO_TO_PREVIOUS_PAGE, GO_TO_MAIN_ACTIVITY;
String ITEM[numberOfItems+1]        = {};  //ITEM NAME AND FORMAT TO DISPLAY
int ACTION[numberOfItems+1]         = {};  //POSSIBLE ACTIONS ON ITEM SELECTION
int ACTIVITY[numberOfItems+1]       = {};  //POSSIBLE NEXT PAGES ON ITEM SELECTION
int ADJUST[numberOfItems+1]         = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF A VARIABLE
int CURRENT_PAGE = MAIN_ACTIVITY;



void setup() {
  Serial.begin(BAUD_RATE);
  //while(!Serial);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if(SETTING.ENABLE_OLED){ 
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.setFontType(0);
    oled.display();         // Display what's in the buffer (splashscreen)
  }
}



void loop() {
  
  unsigned long currentMillis = millis();

    /*#######################################*/
   ////////////////MAIN ACIVITY//////////////
  /*######################################*/
  if(CURRENT_PAGE==MAIN_ACTIVITY){
    oled.clear(PAGE);  //Clear the screen
    oled.line(random(0,64), random(0,48), random(0,64), random(0,48));
    BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, currentMillis);
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      SETTING.ENABLE_BEEP=!SETTING.ENABLE_BEEP;
      oled.setCursor(0,0);
      if(!SETTING.ENABLE_BEEP){oled.print("BEEP=OFF");}
      else{oled.print("BEEP=ON");}
      oled.display();
      delay(250);
    }  
    if(BUTTON.PRESS==BUTTON.HOLD){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      CURRENT_PAGE=SETTINGS;
    }
    oled.setCursor(10,20);
    oled.println("MAIN");
    oled.setCursor(5, 30);
    oled.println("ACTIVITY");
    oled.display();   //Draw the new screen
  }


    /*#######################################*/
   ////////////////SETTINGS MENU//////////////
  /*######################################*/    
  ITEM[]={"___SETTINGS (PAGE 1)___",
    "SETTINGS",
    ">BEEP",
    ">BLE",
    ">OLED",
    ">USER",
    "<-*", "->"
  };
  ACTION[] = {0,//POSSIBLE NEXT PAGES WHEN ITEM IS SELECTED;
    NO_ACTION,
    BEEP,
    BLUETOOTH,
    OLED,
    USER,
    THRESHOLD, MAIN_ACTIVITY 
  };
  while(CURRENT_PAGE==SETTINGS){
      
    //CHECK BUTTON STATUS:
    BUTTON.PRESS=BUTTON.CHECK(BUTTON.PIN, millis());
    
    //BUTTON WAS CLICKED; MOVE CURSOR TO THE NEXT ITEM:
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION;
      for(int i=1; i<=numberOfItems; i++){
        if(ITEM[i].endsWith("*")){
          ITEM[i] = ITEM[i].substring(0,ITEM[i].length());
          if(i<=numberOfItems-1){ITEM[i+1]+='*';}
          else{ITEM[1]+='*';}
        }
      }
    }
    
    //FOR ALL ITEM TYPES THAT ALLOW A CURSOR:
    for(int i=1; i<=numberOfItems; i++){
      if(ITEM[i].startsWith(">")      //EXPANDABLE
      || ITEM[i].startsWith("<")      //BACK
      || ITEM[i].startsWith("-")      //DECREMENT OR EXIT
      || ITEM[i].startsWith("+")      //INCREMENT
      || ITEM[i].startsWith("[ON]")   //SWITCH_OFF
      || ITEM[i].startsWith("[OFF]")  //SWITCH_ON
      ){
        //DON'T MOVE;
      }       
      else{
        //MOVE THE CURSOR TO THE NEXT CURSORABLE ITEM:
        ITEM[i] = ITEM[i].substring(0,ITEM[i].length()); 
        if(i<=numberOfItems-1){ITEM[i+1] += '*';}
        else{ITEM[1] += '*';}
      }
    }
      
      
      
      /***************************************************/
      
      //IF A BUTTON IS CLICKED, HIGHLIGHT THE NEXT ITEM:
      if(BUTTON.PRESS==BUTTON.CLICK){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_BEEP.endsWith("*")){_BEEP.replace('*',' '); _BEEP.trim(); _BEEP="  "+_BEEP; _BLUETOOTH+='*';}
        else if(_BLUETOOTH.endsWith("*")){_BLUETOOTH.replace('*',' '); _BLUETOOTH.trim(); _BLUETOOTH="  "+_BLUETOOTH; _DISPLAY+='*';}
        else if(_DISPLAY.endsWith("*")){_DISPLAY.replace('*',' '); _DISPLAY.trim(); _DISPLAY="  "+_DISPLAY; _USER+='*';}
        else if(_USER.endsWith("*")){_USER.replace('*',' '); _USER.trim(); _USER="  "+_USER; _EXIT+='*';}
        else if(_EXIT.endsWith("*")){_EXIT.replace('*',' '); _EXIT.trim(); _EXIT="  "+_EXIT; _SETTINGS+='*';}
        else if(_SETTINGS.endsWith("*")){_SETTINGS.replace('*',' '); _SETTINGS.trim(); _BEEP+='*';}
      }
      
      //IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
      if(BUTTON.PRESS==BUTTON.HOLD){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_BEEP.endsWith("*")){CURRENT_PAGE=BEEP;}
        else if(_BLUETOOTH.endsWith("*")){CURRENT_PAGE=BLUETOOTH;}
        else if(_DISPLAY.endsWith("*")){CURRENT_PAGE=OLED;}
        else if(_USER.endsWith("*")){CURRENT_PAGE=USER;}
        else if(_EXIT.endsWith("*")){CURRENT_PAGE=MAIN_ACTIVITY;}
        else if(_SETTINGS.endsWith("*")){CURRENT_PAGE=MAIN_ACTIVITY;}
      }
      
    //REDRAW THE PAGE:
    oled.clear(PAGE);
    oled.setCursor(0,0);
    for(int i=1; i<=numberOfItems; i++){
      if(i<=numberOfItems-2){oled.println(ITEM[i]);}
      else{
        oled.setCursor(0,48); oled.print(ITEM[7]);
        oled.setCursor(36,48); oled.print(ITEM[8]);
      }
    }
    oled.display();
  }



    /*#######################################*/
   /////////////////BEEP MENU////////////////
  /*######################################*/  
  ITEM[]={"___BEEP MENU (PAGE 2)___",
    "BEEP",
    ">VOL*",
    ">THRESH",
    ">PITCH",
    " ",
    "<-",  "->"
  };
  ACTION(0,//POSSIBLE NEXT PAGES WHEN ITEM IS SELECTED;
    NO_ACTION,
    VOL,
    THRESHOLD,
    PITCH,
    NO_ACTION,
    SETTINGS, MAIN_ACTIVITY 
  );    
    while(CURRENT_PAGE==BEEP){
      unsigned long buttMillis = millis();
      BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, buttMillis);
      //IF A BUTTON IS CLICKED, HIGHLIGHT THE NEXT ITEM:
      if(BUTTON.PRESS==BUTTON.CLICK){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_VOLUME.endsWith("*")){_VOLUME.replace('*',' '); _VOLUME.trim(); _VOLUME="  "+_VOLUME; _THRESHOLD+='*';}
        else if(_THRESHOLD.endsWith("*")){_THRESHOLD.replace('*',' '); _THRESHOLD.trim(); _THRESHOLD="  "+_THRESHOLD; _PITCH+='*';}
        else if(_PITCH.endsWith("*")){_PITCH.replace('*',' '); _PITCH.trim(); _PITCH="  "+_PITCH; _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _BEEP+='*';}
        else if(_BEEP.endsWith("*")){_BEEP.replace('*',' '); _BEEP.trim(); _VOLUME+='*';}
      }
      //IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
      if(BUTTON.PRESS==BUTTON.HOLD){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_VOLUME.endsWith("*")){CURRENT_PAGE=VOLUME;}
        else if(_THRESHOLD.endsWith("*")){CURRENT_PAGE=THRESHOLD;}
        else if(_PITCH.endsWith("*")){CURRENT_PAGE=PITCH;}
        else if(_SAVEANDEXIT.endsWith("*")){CURRENT_PAGE=MAIN_ACTIVITY;}
        else if(_BEEP.endsWith("*")){CURRENT_PAGE=SETTINGS;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_BEEP);
      oled.println(_VOLUME);
      oled.println(_THRESHOLD);
      oled.println(_PITCH);
      oled.println(_BLANK);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }









//===================================================================================================  
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
//****************************** BASE ALL ACTIVITIES ON VOLUME MENU *********************************
/*/////////////////////////////////////////////////////////////////////////////////////////////////*/
//===================================================================================================
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv


    /*#######################################*/
   ////////////////VOLUME MENU///////////////
  /*######################################*/  
  ITEM("___VOLUME MENU (PAGE 3)___",
    "VOL=100",  //TODO-- OBTAIN A GIVEN VOLUME SETTING AT STARTUP;
    "+25*",
    "-25",
    "[ON]",
    " ",
    "<-", "->"
  );
  ACTION(0,      //POSSIBLE ACTIONS WHEN CORRESPONDING ITEM IS SELECTED;
    NO_ACTION,
    INCREMENT,
    DECREMENT,
    TOGGLE,
    NO_ACTION,
    CHANGE_ACTIVITY, CHANGE_ACTIVITY
  );
  ACTIVITY(0,    //POSSIBLE NEXT PAGES WHEN CORRESPONDING ITEM IS SELECTED;
    NO_ACTIVITY,
    NO_ACTIVITY,
    NO_ACTIVITY,
    NO_ACTIVITY,
    NO_ACTIVITY,
    BEEP, MAIN_ACTIVITY 
  ); 
  void ADJUST(int& variable, int& varMax, int& varMin, int& varIncrement){
    ADJUST[0] = variable;
    ADJUST[1] = varMax;
    ADJUST[2] = varMin;
    ADJUST[3] = varIncrement;
  }
  ADJUST(        //PARAMETERS TO ADJUST THE VARIABLE DISPLAYED AS ITEM 1;
    SETTING.VOLUME              //[0]
    SETTING.VOLUME_MAX          //[1]
    SETTING.VOLUME_MIN          //[2]
    SETTING.VOLUME_INCREMENT    //[3]
  );
  while(CURRENT_PAGE==VOL){
      
    //CHECK BUTTON STATUS:
    BUTTON.PRESS=BUTTON.CHECK(BUTTON.PIN, millis());
    
    //BUTTON WAS CLICKED; MOVE CURSOR TO THE NEXT ITEM:
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION;
      for(int i=1; i<=numberOfItems; i++){
        if(ITEM[i].endsWith("*")){
          ITEM[i] = ITEM[i].substring(0,ITEM[i].length());
          if(i<=numberOfItems-1){ITEM[i+1]+='*';}
          else{ITEM[1]+='*';}
        }
      }
    }
    
    //FOR ALL ITEM TYPES THAT DON'T ALLOW A CURSOR:
    for(int i=1; i<=numberOfItems; i++){
      if(ACTION[i]==NO_ACTION){
        //MOVE THE CURSOR TO THE NEXT CURSORABLE ITEM:
        ITEM[i] = ITEM[i].substring(0,ITEM[i].length()); 
        if(i<=numberOfItems-1){ITEM[i+1] += '*';}
        else{ITEM[1] += '*';}
      }
    }
      
      
    //TODO-- IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
    if(BUTTON.PRESS==BUTTON.HOLD){
      BUTTON.PRESS=BUTTON.NO_ACTION;
    
      //LOOK AT EACH ITEM IN THE LIST:
      for(int i=1; i<=numberOfItems; i++){
        
        //IDENTIFY THE ITEM THAT HAS THE CURSOR:
        if(ITEM[i].endsWith("*")){

          /*************************************************
          //DECIDE ON AN ACTION TO TAKE WHEN ITEM IS SELECTED:
          /*************************************************/
          
          if(ACTION[i]==INCREMENT){
            if(ITEM[i].startsWith("+")){
              ITEM[i]+=SETTING.VOLUME_INCREMENT; 
              if(ITEM[i]>SETTING.VOLUME_MAX){SETTING.VOLUME=SETTING.VOLUME_MAX;} 
            }
            //TODO-- UPDATE VALUE:
            //ITEM[1].indexOf("=")+1; 
            //ITEM[i].substring(0,ITEM[i].indexOf("=")+1);
          }

          
          if(ACTION[i]==DECREMENT){              
            if(ITEM[i].startsWith("-")){
              ITEM[i]-=SETTING.VOLUME_INCREMENT; 
              if(ITEM[i]<SETTING.VOLUME_MIN){SETTING.VOLUME=SETTING.VOLUME_MIN;} 
            }
            //TODO-- UPDATE VALUE:
            //ITEM[1].indexOf("=")+1; 
            //ITEM[i].substring(0,ITEM[i].indexOf("=")+1);
          }
             

          if(ACTION[i]==TOGGLE){
            if(ITEM[i]=="[OFF]"){ITEM[i]=="[ON]";}
            else{ITEM[i]=="[OFF]";}
            //TODO-- STORE VALUE;
          }

          if(ACTION[i]==CHANGE_ACTIVITY){
            //GO TO NEXT PAGE:
            CURRENT_PAGE = ACTIVITY[i];
          }
           
        }
      }
    }

      
    //REDRAW THE PAGE:
    oled.clear(PAGE);
    oled.setCursor(0,0);
    for(int i=1; i<=numberOfItems; i++){
      if(i<=numberOfItems-2){oled.println(ITEM[i]);}
      else{
        oled.setCursor(0,48); oled.print(ITEM[7]);
        oled.setCursor(36,48); oled.print(ITEM[8]);
      }
    }
    oled.display();
  }


//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  
//===================================================================================================  
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
//***************************************************************************************************
/*/////////////////////////////////////////////////////////////////////////////////////////////////*/
//===================================================================================================



      /*#######################################*/
     ///////////////THRESHOLD MENU/////////////
    /*######################################*/   
           _THRESHOLD =     "THRESHLD";   //title
    String _CLIMB =         "  >CLIMB*";  //expandable
    String _SINK =          "  >SINK";    //expandable
           _BLANK =         "  ";         //blank
           _BLANK =         "  ";         //blank
           _SAVEANDEXIT =   "  EXIT";     //exit
    while(CURRENT_PAGE==THRESHOLD){
      unsigned long buttMillis = millis();
      BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, buttMillis);
      //IF A BUTTON IS CLICKED, HIGHLIGHT THE NEXT ITEM:
      if(BUTTON.PRESS==BUTTON.CLICK){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_CLIMB.endsWith("*")){_CLIMB.replace('*',' '); _CLIMB.trim(); _CLIMB="  "+_CLIMB; _SINK+='*';}
        else if(_SINK.endsWith("*")){_SINK.replace('*',' '); _SINK.trim(); _SINK="  "+_SINK; _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _THRESHOLD+='*';}
        else if(_THRESHOLD.endsWith("*")){_THRESHOLD.replace('*',' '); _THRESHOLD.trim(); _CLIMB+='*';}
      }
      //IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
      if(BUTTON.PRESS==BUTTON.HOLD){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_CLIMB.endsWith("*")){CURRENT_PAGE=CLIMB;}
        else if(_SINK.endsWith("*")){CURRENT_PAGE=SINK;}
        else if(_SAVEANDEXIT.endsWith("*")){CURRENT_PAGE=MAIN_ACTIVITY;}
        else if(_THRESHOLD.endsWith("*")){CURRENT_PAGE=BEEP;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_THRESHOLD);
      oled.println(_CLIMB);
      oled.println(_SINK);
      oled.println(_BLANK);
      oled.println(_BLANK);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }

      /*#######################################*/
     ////////////////CLIMB MENU////////////////
    /*######################################*/    
           _CLIMB =       "CLIMB=1";
    String _UP1FT =         "  +1ft*";
    String _DOWN1FT =       "  -1ft";
           _BLANK =         "  ";
           _BLANK =         "  ";
           _SAVEANDEXIT =  "  EXIT";     
    while(CURRENT_PAGE==CLIMB){
      if(_CLIMB.endsWith("*")){_CLIMB = "CLIMB="+String(round(SETTING.CLIMB_BEEP_TRIGGER)); _CLIMB+="*";}
      else{_CLIMB = "CLIMB="+String(round(SETTING.CLIMB_BEEP_TRIGGER));}  
      unsigned long buttMillis = millis();
      BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, buttMillis);
      //BUTTON IS CLICKED; DO THINGS:
      if(BUTTON.PRESS==BUTTON.CLICK){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_UP1FT.endsWith("*")){_UP1FT.replace('*',' '); _UP1FT.trim(); _UP1FT="  "+_UP1FT; _DOWN1FT+='*';}
        else if(_DOWN1FT.endsWith("*")){_DOWN1FT.replace('*',' '); _DOWN1FT.trim(); _DOWN1FT="  "+_DOWN1FT; _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _CLIMB+='*';}
        else if(_CLIMB.endsWith("*")){_CLIMB.replace('*',' '); _CLIMB.trim(); _UP1FT+='*';}
      }
      //BUTTON IS HELD FOR MORE THAN ONE SECOND; DO THINGS:
      if(BUTTON.PRESS==BUTTON.HOLD){
        BUTTON.PRESS=BUTTON.NO_ACTION;
        if(_UP1FT.endsWith("*")){_CLIMB=_CLIMB.substring(6); int _CLM=_CLIMB.toFloat(); _CLM+=1; if(_CLM>10){_CLM=10;} SETTING.CLIMB_BEEP_TRIGGER=_CLM; _CLIMB="CLIMB="+String(round(_CLM));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_DOWN1FT.endsWith("*")){_CLIMB=_CLIMB.substring(6); int _CLM=_CLIMB.toFloat(); _CLM-=1; if(_CLM<1){_CLM=1;} SETTING.CLIMB_BEEP_TRIGGER=_CLM; _CLIMB="CLIMB="+String(round(_CLM));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_SAVEANDEXIT.endsWith("*")){CURRENT_PAGE=MAIN_ACTIVITY;}
        else if(_CLIMB.endsWith("*")){CURRENT_PAGE=THRESHOLD;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_CLIMB);
      oled.println(_UP1FT);
      oled.println(_DOWN1FT);
      oled.println(_BLANK);
      oled.println(_BLANK);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }



    /**************************************
    THE_MENU.MENU_ACTIVITY(
      THE_MENU.SINK, THE_MENU.THRESHOLD, 
      SETTING.SINK_ALARM_TRIGGER, 1,
      -10, 1, 
      THE_MENU.DISPLAYED_VALUE, "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER)), THE_MENU.NO_NEXT,
      THE_MENU.CLICKABLE, "  +1ft/s*", THE_MENU.NO_NEXT,
      THE_MENU.CLICKABLE, "  -1ft/s", THE_MENU.NO_NEXT,
      THE_MENU.BLANK, " ", THE_MENU.NO_NEXT,
      THE_MENU.BLANK, " ", THE_MENU.NO_NEXT
    );
    **************************************/ 

    



      /*#######################################*/
     /////////////////SINK MENU////////////////
    /*######################################*/   
      
  TYPE[numberOfItems+1] = {  0,                  DISPLAYED_VALUE,    CLICKABLE,      CLICKABLE,      BLANK,       BLANK,       BACK,             EXIT          };
  ITEM[numberOfItems+1] = {  "PLACE_HOLDER",     "SINK=-1",          "  +1ft/s*",    "  -1ft/s",     "  ",        "  ",        "<-",             "->"          };
  NEXT[numberOfItems+1] = {  0,                  NO_NEXT,            NO_NEXT,        NO_NEXT,        NO_NEXT,     NO_NEXT,     THRESHOLD,        MAIN_ACTIVITY };

  while(CURRENT_PAGE==SINK){
    
    //TODO--OBTAIN NECESSARY DEFAULT OR EXISTING VALUES:
    /*if(itemOne.endsWith("*")){itemOne = "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER)); itemOne+="*";}  */
    /*else{itemOne = "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER));}  */

    //CHECK BUTTON STATUS:
    BUTTON.PRESS=BUTTON.CHECK(BUTTON.PIN, millis());
    
    //BUTTON WAS CLICKED; DO THINGS:
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION;
      for(int i=1; i<=numberOfItems; i++){
        if(ITEM[i].endsWith("*")){
          ITEM[i] = ITEM[i].substring(0,ITEM[i].length());
          if(i<=numberOfItems-1){ITEM[i+1]+='*';}
          else{ITEM[1]+='*';}
        }
      }
    }
    
    //FOR ALL ITEM TYPES THAT SHOULD NOT HAVE A CURSOR:
    for(int i=1; i<=numberOfItems; i++){
      if(ITEM[i].endsWith("*") && (TYPE[i]==BLANK || TYPE[i]==TITLE || TYPE[i]==DISPLAYED_VALUE)){
        ITEM[i] = ITEM[i].substring(0,ITEM[i].length()); 
        if(i<=numberOfItems-1){ITEM[i+1] += '*';}
        else{ITEM[1] += '*';}
      }
    }
    
    
    //BUTTON WAS HELD; DO THINGS:
    if(BUTTON.PRESS==BUTTON.HOLD){
      BUTTON.PRESS=BUTTON.NO_ACTION;
      //LOOK AT EACH ITEM IN THE LIST:
      for(int i=1; i<=numberOfItems; i++){
        //IDENTIFY THE ITEM THAT HAS THE CURSOR:
        if(ITEM[i].endsWith("*")){
          //IDENTIFY THE ITEM TYPE:
          int VAL;
          String _noVAL;
          switch(TYPE[i]){
            
            //UPDATE DISPLAYED_VALUE:
            case 3/*CLICKABLE*/: 
            
//            int _lengthNoVal=ITEM[i].indexOf("=")+1; 
            _noVAL=ITEM[i].substring(0,ITEM[i].indexOf("=")+1); 
//            String _sVAL=ITEM[i].substring(_lengthNoVal); 
//            int _VAL=_sVAL.toFloat(); 
            VAL=displayedValue;
            if(_noVAL.startsWith("  +")){
              VAL+=displayedValueIncrement; 
              if(VAL>displayedValueMax){VAL=displayedValueMax;} 
            }
            else if(_noVAL.startsWith("  -")){
              VAL-=displayedValueIncrement; 
              if(VAL<displayedValueMin){VAL=displayedValueMin;} 
            }
            
            //SETTING.SINK_ALARM_TRIGGER=_VAL; 
            ITEM[i]=_noVAL+String(round(VAL));  
            //TODO-- STORE THE NEW VALUE:
            /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/
            break;
            
            //GO TO NEXT PAGE:
            case 4/*EXPANDABLE*/: CURRENT_PAGE = NEXT[i]; 
            break;   

            //MARK CURRENT ITEM AND UN-MARK SPECIFIED OTHER:
            case 5/*CHECKABLE*/: 
            break;
            
            //GO TO PREVIOUS PAGE:
            case BACK: CURRENT_PAGE = PREVIOUS_PAGE; 
            break;   
            
            //GO TO MAIN ACTIVITY:
            case EXIT: CURRENT_PAGE = MAIN_ACTIVITY; 
            break; 
            
          };
        }
      }

      //TO UPDATE DISPLAYED_VALUE, DO SOMETHING SIMILAR TO THE FOLLOWING:
      /**/
      //if(_LINE2.endsWith("*")){int _lengthNoVal=_LINE1.indexOf("=")+1; String _noVAL=_LINE1.substring(0,_lengthNoVal); String _sVAL=_LINE1.substring(_lengthNoVal); int _VAL=_sVAL.toFloat(); _VAL+=1; if(_VAL>-1){_VAL=-1;} SETTING.SINK_ALARM_TRIGGER=_VAL; _LINE1=_noVAL+String(round(_VAL));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
      //if(_LINE3.endsWith("*")){int _lengthNoVal=_LINE1.indexOf("=")+1; String _noVAL=_LINE1.substring(0,_lengthNoVal); String _sVAL=_LINE1.substring(_lengthNoVal); int _VAL=_sVAL.toFloat(); _VAL-=1; if(_VAL<-10){_VAL=-10;} SETTING.SINK_ALARM_TRIGGER=_VAL; _LINE1=_noVAL+String(round(_VAL));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
      /**/

    }

    //REDRAW THE PAGE:
    oled.clear(PAGE);
    oled.setCursor(0,0);
    for(int i=1; i<=numberOfItems; i++){
      if(i<=numberOfItems-2){oled.println(ITEM[i]);}
      else{
        oled.setCursor(0,48); oled.print(ITEM[7]);
        oled.setCursor(36,48); oled.print(ITEM[8]);
      }
    }
    oled.display();
  }


















/*
    
    _LINE1 = "SINK=-1";     //itemType[]=
    _LINE2 = "  +1ft/s*";   //itemType[]=
    _LINE3 = "  -1ft/s";    //itemType[]=
    _LINE4 = "  ";          //itemType[]=
    _LINE5 = "  ";          //itemType[]=
    _LINE6 = "<-";          //itemType[]=
    
    while(THE_MENU.CURRENT_PAGE==THE_MENU.SINK){
      if(_LINE1.endsWith("*")){_LINE1 = "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER)); _LINE1+="*";}
      else{_LINE1 = "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER));}  
      BUTTON.buttonPress=BUTTON.checkButton(BUTTON.PIN, millis());
      //BUTTON IS CLICKED; DO THINGS:
      if(BUTTON.buttonPress==BUTTON.BUTTON_CLICK){
        BUTTON.buttonPress=BUTTON.BUTTON_NO_ACTION;
        if(_LINE2.endsWith("*")){_LINE2.replace('*',' '); _LINE2.trim(); _LINE2="  "+_LINE2; _LINE3+='*';}
        else if(_LINE3.endsWith("*")){_LINE3.replace('*',' '); _LINE3.trim(); _LINE3="  "+_LINE3; _LINE6+='*';}
        else if(_LINE6.endsWith("*")){_LINE6.replace('*',' '); _LINE6.trim(); _LINE6="  "+_LINE6; _LINE1+='*';}
        else if(_LINE1.endsWith("*")){_LINE1.replace('*',' '); _LINE1.trim(); _LINE2+='*';}
      }
      //BUTTON IS HELD FOR MORE THAN ONE SECOND; DO THINGS:
      if(BUTTON.buttonPress==BUTTON.BUTTON_HOLD){
        BUTTON.buttonPress=BUTTON.BUTTON_NO_ACTION;
        if(_LINE2.endsWith("*")){int _lengthNoVal=_LINE1.indexOf("=")+1; String _noVAL=_LINE1.substring(0,_lengthNoVal); String _sVAL=_LINE1.substring(_lengthNoVal); int _VAL=_sVAL.toFloat(); _VAL+=1; if(_VAL>-1){_VAL=-1;} SETTING.SINK_ALARM_TRIGGER=_VAL; _LINE1=_noVAL+String(round(_VAL));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_LINE3.endsWith("*")){int _lengthNoVal=_LINE1.indexOf("=")+1; String _noVAL=_LINE1.substring(0,_lengthNoVal); String _sVAL=_LINE1.substring(_lengthNoVal); int _VAL=_sVAL.toFloat(); _VAL-=1; if(_VAL<-10){_VAL=-10;} SETTING.SINK_ALARM_TRIGGER=_VAL; _LINE1=_noVAL+String(round(_VAL));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_LINE6.endsWith("*")){CURRENT_PAGE=MAIN_ACTIVITY;}
        else if(_LINE1.endsWith("*")){CURRENT_PAGE=THRESHOLD;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_LINE1);
      oled.println(_LINE2);
      oled.println(_LINE3);
      oled.println(_LINE4);
      oled.println(_LINE5);
      oled.println(_LINE6);
      oled.display();
    }
*/

      /*#######################################*/
     ////////////////PITCH MENU////////////////
    /*######################################*/     
    while(CURRENT_PAGE==PITCH){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }


      /*#######################################*/
     ///////////////CLIMB_MAX MENU/////////////
    /*######################################*/     
    while(CURRENT_PAGE==CLIMB_MAX){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }


      /*#######################################*/
     ///////////////CLIMB_MIN MENU/////////////
    /*######################################*/     
    while(CURRENT_PAGE==CLIMB_MIN){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }    


      /*#######################################*/
     ////////////////SINK_MAX MENU/////////////
    /*######################################*/     
    while(CURRENT_PAGE==SINK_MAX){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }      


      /*#######################################*/
     ////////////////SINK_MIN MENU/////////////
    /*######################################*/     
    while(CURRENT_PAGE==SINK_MIN){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }           
        
    
      /*#######################################*/
     ///////////////BLUETOOTH MENU/////////////
    /*######################################*/     
    while(CURRENT_PAGE==BLUETOOTH){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }         


      /*#######################################*/
     /////////////////OLED MENU////////////////
    /*######################################*/     
    while(CURRENT_PAGE==OLED){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     //////////////CHART_SPEED MENU////////////
    /*######################################*/     
    while(CURRENT_PAGE==CHART_SPEED){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     ////////////////SINK_MAX MENU/////////////
    /*######################################*/     
    while(CURRENT_PAGE==SINK_MAX){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     



      /*#######################################*/
     //////////////////USER MENU///////////////
    /*######################################*/     
    while(CURRENT_PAGE==USER){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     ///////////////EDIT_NAMES MENU////////////
    /*######################################*/     
    while(CURRENT_PAGE==EDIT_NAMES){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     ////////////////USER_1 MENU///////////////
    /*######################################*/     
    while(CURRENT_PAGE==USER_1){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     
    

      /*#######################################*/
     ////////////////USER_2 MENU///////////////
    /*######################################*/     
    while(CURRENT_PAGE==USER_2){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }



      /*#######################################*/
     ////////////////USER_3 MENU///////////////
    /*######################################*/     
    while(CURRENT_PAGE==USER_3){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }





    
}
