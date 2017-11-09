#include "DEFAULT_SETTINGS.h"
/*#include "MENU.h"*/
#include "BUTTON.h"
#include "OLED.h"

#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define BUTTON_PIN                    A1    // CONNECT THE OTHER BUTTON LEAD TO GROUND;
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

BUTTON BUTTON;
Default SETTING;
/*MENU THE_MENU;*/
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);


//POSSIBLE ACTIVITY NAMES:
enum {
  MAIN_ACTIVITY,
  SETTINGS,
  BEEP,
  VOL,
  THRESHOLD,
  CLIMB,
  SINK,
  PITCH,
  CLIMB_MAX,
  CLIMB_MIN,
  SINK_MAX,
  SINK_MIN,
  BLUETOOTH,
  OLED,
  CHART_SPEED,
  USER,
  EDIT_NAMES,
  USER_1,
  USER_2,
  USER_3
};


//POSSIBLE ITEM PURPOSES:
enum {NONE, NAMED_INT, NAMED_FLOAT, INT_ADJUSTER, FLOAT_ADJUSTER, BOOL_TOGGLER, ACTIVITY_CHANGER};

//FILL THESE ARRAYS FOR ORGANIZATION:
#define numberOfItems                   7  //ITEM_1, ITEM_2, ITEM_3, ITEM_4, ITEM5, GO_TO_PREVIOUS_PAGE, GO_TO_MAIN_ACTIVITY;
String ITEM[numberOfItems+1]        = {};  //ITEM NAME AND FORMAT TO DISPLAY
int PURPOSE[numberOfItems+1]        = {};  //POSSIBLE PURPOSES ON ITEM SELECTION
int ACTIVITY[numberOfItems+1]       = {};  //POSSIBLE NEXT PAGES ON ITEM SELECTION
int ADJUST[numberOfItems+1]         = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF A VARIABLE
int CURRENT_PAGE = MAIN_ACTIVITY;

void menuItem(String item1, String item2, String item3, String item4, String item5, String item6, String item7);
void itemPurpose(int purpose1, int purpose2, int purpose3, int purpose4, int purpose5, int purpose6, int purpose7);
void ifSelectedGoTo(int activity1, int activity2, int activity3, int activity4, int activity5, int activity6, int activity7);
void integerAdjust(int& integerToAdjust, int integerMin, int integerMax);
void floatAdjust(float& floatToAdjust, float floatMin, float floatMax);
void booleanToggle(bool& booleanToToggle);
void startActivity(int pageNumber);


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
  while(CURRENT_PAGE==MAIN_ACTIVITY){
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
  menuItem(
    "SETTINGS",  
    ">BEEP*",
    ">BTOOTH",
    ">DISPLAY",
    ">USER",
    "<-", "->"
  );
  itemPurpose(NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER);
  ifSelectedGoTo(NONE, BEEP, BLUETOOTH, OLED, USER, MAIN_ACTIVITY, MAIN_ACTIVITY);
  startActivity(SETTINGS);


    /*#######################################*/
   /////////////////BEEP MENU////////////////
  /*######################################*/    
  menuItem(
    "BEEP",  
    ">VOLUME*",
    ">THRESH",
    ">PITCH",
    " ",
    "<-", "->"
  );
  itemPurpose(NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER);
  ifSelectedGoTo(NONE, VOL, THRESHOLD, PITCH, NONE, SETTINGS, MAIN_ACTIVITY);
  startActivity(BEEP);


    /*#######################################*/
   ////////////////VOLUME MENU///////////////
  /*######################################*/  
  menuItem(//NAMED_INT AND NAMED_FLOAT, AND BOOL_TOGGLER WILL AUTOMATICALLY UPDATE TO THEIR PROPER VALUE;
    "VOL=100",  
    "+25*",
    "-25",
    "[ON]",
    " ",
    "<-", "->"
  );
  itemPurpose(NAMED_INT, INT_ADJUSTER, INT_ADJUSTER, BOOL_TOGGLER, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER);   //POSSIBLE PURPOSES WHEN CORRESPONDING ITEM IS SELECTED;
  ifSelectedGoTo(NONE, NONE, NONE, NONE, NONE, BEEP, MAIN_ACTIVITY);                                            //POSSIBLE NEXT PAGES WHEN CORRESPONDING ITEM IS SELECTED;
  integerAdjust(SETTING.VOLUME, SETTING.VOLUME_MIN, SETTING.VOLUME_MAX);                                        //PARAMETERS TO ADJUST THE NAMED_INT VALUE DISPLAYED;
  booleanToggle(SETTING.ENABLE_BEEP);                                                                           //BOOLEAN VARIABLE TO SWAP BETWEEN "[ON]" AND "[OFF]";
  startActivity(VOL);                                                                                           //START THE ACTIVITY;


    /*#######################################*/
   ///////////////THRESHOLD MENU/////////////
  /*######################################*/   
  while(CURRENT_PAGE==THRESHOLD){

  }

    /*#######################################*/
   ////////////////CLIMB MENU////////////////
  /*######################################*/    
  while(CURRENT_PAGE==CLIMB){
    
  }


    /*#######################################*/
   /////////////////SINK MENU////////////////
  /*######################################*/   
  while(CURRENT_PAGE==SINK){

  }


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
//  MENU_ITEM();
//  MENU_ACTION();
//  MENU_ACTIVITY();
//  MENU_ADJUST();
//  MENU_START(SINK_MAX);    



    /*#######################################*/
   //////////////////USER MENU///////////////
  /*######################################*/     
//  MENU_ITEM();
//  MENU_ACTION();
//  MENU_ACTIVITY();
//  MENU_ADJUST();
//  MENU_START(USER);    


    /*#######################################*/
   ///////////////EDIT_NAMES MENU////////////
  /*######################################*/     
//  MENU_ITEM();
//  MENU_ACTION();
//  MENU_ACTIVITY();
//  MENU_ADJUST();
//  MENU_START(EDIT_NAMES);  


    /*#######################################*/
   ////////////////USER_1 MENU///////////////
  /*######################################*/     
//  MENU_ITEM();
//  MENU_ACTION();
//  MENU_ACTIVITY();
//  MENU_ADJUST();
//  MENU_START(USER_1);    
    

    /*#######################################*/
   ////////////////USER_2 MENU///////////////
  /*######################################*/     
//  MENU_ITEM();
//  MENU_ACTION();
//  MENU_ACTIVITY();
//  MENU_ADJUST();
//  MENU_START(USER_2);



    /*#######################################*/
   ////////////////USER_3 MENU///////////////
  /*######################################*/     
//  MENU_ITEM();
//  MENU_ACTION();
//  MENU_ACTIVITY();
//  MENU_ADJUST();
//  MENU_START(USER_3);


}


void menuItem(String item1, String item2, String item3, String item4, String item5, String item6, String item7){
  ITEM[1] = item1;
  ITEM[2] = item2;
  ITEM[3] = item3;
  ITEM[4] = item4;
  ITEM[5] = item5;
  ITEM[6] = item6;
  ITEM[7] = item7;
}


void itemPurpose(int purpose1, int purpose2, int purpose3, int purpose4, int purpose5, int purpose6, int purpose7){
  PURPOSE[1] = purpose1;
  PURPOSE[2] = purpose2;
  PURPOSE[3] = purpose3;
  PURPOSE[4] = purpose4;
  PURPOSE[5] = purpose5;
  PURPOSE[6] = purpose6;
  PURPOSE[7] = purpose7;
}


void ifSelectedGoTo(int activity1, int activity2, int activity3, int activity4, int activity5, int activity6, int activity7){
  ACTIVITY[1] = activity1;
  ACTIVITY[2] = activity2;
  ACTIVITY[3] = activity3;
  ACTIVITY[4] = activity4;
  ACTIVITY[5] = activity5;
  ACTIVITY[6] = activity6;
  ACTIVITY[7] = activity7;
}


void integerAdjust(int& integerToAdjust, int integerMin, int integerMax){
  ADJUST[0] = integerToAdjust;
  ADJUST[1] = integerMin;
  ADJUST[2] = integerMax;
}


void floatAdjust(float& floatToAdjust, float floatMin, float floatMax){
  ADJUST[0] = floatToAdjust;
  ADJUST[1] = floatMin;
  ADJUST[2] = floatMax;
}


void booleanToggle(bool& booleanToToggle){
  ADJUST[0] = booleanToToggle;
}

void startActivity(int pageNumber){
  Serial.print("CURRENT_PAGE=="); Serial.println(CURRENT_PAGE);
  
 
  enum {_VARIABLE, _MIN, _MAX};

  //INITIALIZE NAMED_INT, NAMED_FLOAT, AND BOOL_TOGGLER TO PROPER VALUE:
  for(int i=1; i<=numberOfItems; i++){    
    if(PURPOSE[i]==NAMED_INT || PURPOSE[i]==NAMED_FLOAT){
      ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(ADJUST[_VARIABLE]);
    }
    if(PURPOSE[i]==BOOL_TOGGLER){
      if(ITEM[i].endsWith("*")){
        if(ADJUST[_VARIABLE]){ITEM[i] = "[ON]*";}
        else if(!ADJUST[_VARIABLE]){ITEM[i] = "[OFF]*";}
      }
      else{
        if(ADJUST[_VARIABLE]){ITEM[i] = "[ON]";}
        else if(!ADJUST[_VARIABLE]){ITEM[i] = "[OFF]";}
      }
      
    }
  }

  //CHECK WHAT HAS BEEN INITIALIZED:
  for(int i=1; i<=numberOfItems; i++){Serial.println(ITEM[i]);}
  
  while(CURRENT_PAGE==pageNumber){
     
    //CHECK BUTTON STATUS:
    BUTTON.PRESS=BUTTON.CHECK(BUTTON.PIN, millis());
    
    //BUTTON WAS CLICKED; MOVE CURSOR TO THE NEXT ITEM:
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION;

      //LOOK THROUGH EACH ITEM:
      int i;
      for(i=1; i<=numberOfItems; i++){
        //IDENTIFY THE LOCATION OF THE CURSOR:
        if(ITEM[i].endsWith("*")){
          ITEM[i].remove(ITEM[i].length()-1);
          break;  //VALUE OF i IS THE LOCATION OF THE CURSOR;
        }  
      }
      //IDENTIFY LOCATION OF NEXT CURSORABLE ITEM:
      int j=i+1;
      while(true){
        if(j>numberOfItems){j=1;}
        //IF THE NEXT ITEM [j] HAS NO PURPOSE, LOOK AT THE NEXT [j]:
        if(PURPOSE[j]==NONE){j++;}
        else{ITEM[j]+='*'; break;}  //VALUE OF j IS THE LOCATION OF THE NEXT CURSORABLE ITEM;
      }

      //DEBUG:
      for(int I=1; I<=numberOfItems; I++){Serial.println(ITEM[I]);}
      
    }
    
    //IF A BUTTON IS HELD FOR MORE THAN HALF A SECOND, SELECT/TOGGLE THE ITEM:
    if(BUTTON.PRESS==BUTTON.HOLD){
      BUTTON.PRESS=BUTTON.NO_ACTION;
    
      //LOOK AT EACH ITEM IN THE LIST:
      for(int i=1; i<=numberOfItems; i++){
        
        //IDENTIFY THE ITEM THAT HAS THE CURSOR:
        if(ITEM[i].endsWith("*")){


          /*************************************************************************************
            DECIDE ON AN ACTION TO TAKE WHEN ITEM IS SELECTED ACCORDING TO THE ITEM'S PURPOSE:
          /*************************************************************************************/

          if(PURPOSE[i]==INT_ADJUSTER){
            if(ITEM[i].startsWith("+")){
              ADJUST[_VARIABLE] += (ITEM[i].substring(1)).toInt();
              if(ADJUST[_VARIABLE] > ADJUST[_MAX]){ADJUST[_VARIABLE] = ADJUST[_MAX];} 
            }
            if(ITEM[i].startsWith("-")){
              ADJUST[_VARIABLE] -= (ITEM[i].substring(1)).toInt();
              if(ADJUST[_VARIABLE] < ADJUST[_MIN]){ADJUST[_VARIABLE] = ADJUST[_MIN];} 
            }            
            //UPDATE NAMED_INT VALUE:
            for(int i=1; i<=numberOfItems; i++){
              if(PURPOSE[i]==NAMED_INT){
                ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(ADJUST[_VARIABLE]);
              }
            }
          }

          if(PURPOSE[i]==FLOAT_ADJUSTER){
            if(ITEM[i].startsWith("+")){
              ADJUST[_VARIABLE] += (ITEM[i].substring(1)).toInt();
              if(ADJUST[_VARIABLE] > ADJUST[_MAX]){ADJUST[_VARIABLE] = ADJUST[_MAX];} 
            }
            if(ITEM[i].startsWith("-")){
              ADJUST[_VARIABLE] -= (ITEM[i].substring(1)).toInt();
              if(ADJUST[_VARIABLE] < ADJUST[_MIN]){ADJUST[_VARIABLE] = ADJUST[_MIN];} 
            }            
            //UPDATE NAMED_FLOAT VALUE:
            for(int i=1; i<=numberOfItems; i++){
              if(PURPOSE[i]==NAMED_FLOAT){
                ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + ADJUST[_VARIABLE];
              }
            }
          }

          if(PURPOSE[i]==BOOL_TOGGLER){
            if(ITEM[i]=="[OFF]*"){ITEM[i]=="[ON]*";}
            else{ITEM[i]=="[OFF]*";}
          }

          if(PURPOSE[i]==ACTIVITY_CHANGER){
            CURRENT_PAGE = ACTIVITY[i];
          }
           
        }
      }

      //DEBUG:
      for(int I=1; I<=numberOfItems; I++){Serial.println(ITEM[I]);}
      
    }

      
    //REDRAW THE PAGE:
    oled.clear(PAGE);
    oled.setCursor(0,0);
    for(int i=1; i<=numberOfItems; i++){
      if(i<=numberOfItems-2){oled.println(ITEM[i]);}
      else{
        oled.setCursor(0,41); oled.print(ITEM[6]);
        oled.setCursor(36,41); oled.print(ITEM[7]);
      }
    }
    oled.display();
  }
}
