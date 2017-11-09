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
enum {NONE, NAMED_INT, NAMED_FLOAT, INT_ADJUSTER, ITEM_SWITCHER, FLOAT_ADJUSTER, BOOL_TOGGLER, ACTIVITY_CHANGER};

//FILL THESE ARRAYS FOR ORGANIZATION:
#define numberOfItems                       7  //ITEM_1, ITEM_2, ITEM_3, ITEM_4, ITEM5, GO_TO_PREVIOUS_PAGE, GO_TO_MAIN_ACTIVITY;
String ITEM[numberOfItems+1]            = {};  //ITEM NAME AND FORMAT TO DISPLAY
int PURPOSE[numberOfItems+1]            = {};  //POSSIBLE PURPOSES ON ITEM SELECTION
int ACTIVITY[numberOfItems+1]           = {};  //POSSIBLE NEXT PAGES ON ITEM SELECTION
int INT_ADJUST[numberOfItems+1]         = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF AN INTEGER VARIABLE
int FLOAT_ADJUST[numberOfItems+1]       = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF A FLOATING POINT VARIABLE
String BOOL_DISPLAYED[numberOfItems+1]  = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF A BOOLEAN VARIABLE: "[ON]" OR "[OFF]"
int CURRENT_PAGE = MAIN_ACTIVITY;

void menuItem(String item1, String item2, String item3, String item4, String item5, String item6, String item7);
void itemPurpose(int purpose1, int purpose2, int purpose3, int purpose4, int purpose5, int purpose6, int purpose7);
void ifSelectedGoTo(int activity1, int activity2, int activity3, int activity4, int activity5, int activity6, int activity7);
void integerAdjust(int integerMin, int integerMax);
void floatAdjust(float floatMin, float floatMax);
void booleanToggle(String displayForTrue, String displayForFalse);
void startActivity(int pageNumber, int& integerToAdjust, float& floatToAdjust, bool& boolToAdjust);
int NO_INT = 0;
float NO_FLOAT = 0;
bool NO_BOOL = 0;

bool displayOn = true;

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
    if(SETTING.ENABLE_OLED){
      displayOn = true;
      oled.clear(PAGE);  //Clear the screen
      oled.line(random(0,64), random(0,48), random(0,64), random(0,48));
    }
    BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, currentMillis);
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      SETTING.ENABLE_BEEP=!SETTING.ENABLE_BEEP;
      oled.clear(PAGE);
      oled.setCursor(0,0);
      if(!SETTING.ENABLE_BEEP){oled.print("BEEP=OFF");}
      else{oled.print("BEEP=ON");}
      oled.display();
      delay(250);
      oled.clear(PAGE);
      oled.display();
    }  
    if(BUTTON.PRESS==BUTTON.HOLD){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      CURRENT_PAGE=SETTINGS;
    }
    if(SETTING.ENABLE_OLED){
      oled.setCursor(10,20);
      oled.println("MAIN");
      oled.setCursor(5, 30);
      oled.println("ACTIVITY");
      oled.display();   //Draw the new screen
    }
    else if(displayOn){
      displayOn = false;
      oled.clear(PAGE);
      oled.display();
    }
  }

    /*#######################################*/
   ////////////////SETTINGS MENU//////////////
  /*######################################*/   
  menuItem( "SETTINGS", ">BEEP", ">BTOOTH", ">DISPLAY", ">USER", "<-", "->*" );
  itemPurpose(NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER);
  ifSelectedGoTo(NONE, BEEP, BLUETOOTH, OLED, USER, MAIN_ACTIVITY, MAIN_ACTIVITY);
  startActivity(SETTINGS, NO_INT, NO_FLOAT, NO_BOOL);


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
  startActivity(BEEP, NO_INT, NO_FLOAT, NO_BOOL);


    /*#######################################*/
   ////////////////VOLUME MENU///////////////
  /*######################################*/  
  menuItem(//NAMED_INT AND NAMED_FLOAT, AND BOOL_TOGGLER WILL AUTOMATICALLY UPDATE TO THEIR PROPER VALUE;
    "VOL=THE_VALUE_AFTER_THE_EQUAL_SIGN_WILL_RE-INITIALIZE_AUTOMATICALLY",
    "+25*",
    "-25",
    "THIS_ITEM_WILL_RE-INITIALIZE_ACCORDING_TO_THE_PARAMETER_IN booleanToggle();",
    " ",
    "<-", "->"
  );
  itemPurpose(NAMED_INT, INT_ADJUSTER, INT_ADJUSTER, BOOL_TOGGLER, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER);   //POSSIBLE PURPOSES WHEN CORRESPONDING ITEM IS SELECTED;
  ifSelectedGoTo(NONE, NONE, NONE, NONE, NONE, BEEP, MAIN_ACTIVITY);                                            //POSSIBLE NEXT PAGES WHEN CORRESPONDING ITEM IS SELECTED;
  integerAdjust(SETTING.VOLUME_MIN, SETTING.VOLUME_MAX);                                                        //PARAMETERS TO ADJUST THE NAMED_INT VALUE DISPLAYED;
  booleanToggle( "(ON)", "(MUTED)" );                                                                           //BOOLEAN VARIABLE TO SWAP BETWEEN TRUE AND FALSE;
  startActivity(VOL, SETTING.VOLUME, NO_FLOAT, SETTING.ENABLE_BEEP);                                            //START THE ACTIVITY;





    /*#######################################*/
   ///////////////THRESHOLD MENU/////////////
  /*######################################*/   

  //LIST OF ITEMS TO DISPLAY IN THIS MENU ACTIVITY:
  menuItem(
    "THRESHOLD",        //RECOMMENDED: NINE OR LESS CHARACTERS LONG, INCLUDING CURSOR IF THE ITEM IS CURSORABLE;
    ">CLIMB*",          //RECOMMENDED: INITIAL CURSOR "*" PLACEMENT;
    ">SINK",
    " ",
    " ",
    "<-",               //RECOMMENDED: "<-";
    "->"                //RECOMMENDED: "->";
  );
  
  //PURPOSES: NONE, ACTIVITY_CHANGER, NAMED_INT, INT_ADJUSTER, NAMED_FLOAT, FLOAT_ADJUSTER, BOOL_TOGGLER; //TODO-- ITEM_SWITCHER;
  itemPurpose(
    NONE,               //RECOMMENDED: NONE, NAMED_INT, OR NAMED_FLOAT;
    ACTIVITY_CHANGER, 
    ACTIVITY_CHANGER, 
    NONE, 
    NONE, 
    ACTIVITY_CHANGER,   //RECOMMENDED: ACTIVITY_CHANGER;
    ACTIVITY_CHANGER    //RECOMMENDED: ACTIVITY_CHANGER;
  );
  
  //ACTIVITIES: NONE, OR ANY ACTIVITY DEFINED;
  ifSelectedGoTo(
    NONE,               //RECOMMENDED: NONE;
    CLIMB, 
    SINK, 
    NONE, 
    NONE, 
    BEEP,               //RECOMMENDED: NAME OF THE PREVIOUS ACTIVITY;
    MAIN_ACTIVITY       //RECOMMENDED: MAIN_ACTIVITY;
  );
  
  //integerAdjust(SETTING_MIN, SETTING_MAX);  //COMMENT THIS LINE IF UNUSED;
  //floatAdjust(SETTING_MIN, SETTING_MAX);    //COMMENT THIS LINE IF UNUSED;                                   
  //booleanToggle( "TRUE", "FALSE" );         //COMMENT THIS LINE IF UNUSED;

  //NAME OF THE ACTIVITY TO START, AND REFERENCED VARIABLES TO ADJUST IF APPLICABLE:
  startActivity(THRESHOLD, NO_INT, NO_FLOAT, NO_BOOL);



    /*#######################################*/
   ////////////////CLIMB MENU////////////////
  /*######################################*/    
  menuItem( "CLIMB=1", "+1*", "-1", " ", " ", "<-", "->" );
  itemPurpose( NAMED_FLOAT, FLOAT_ADJUSTER, FLOAT_ADJUSTER, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, THRESHOLD, MAIN_ACTIVITY );
  floatAdjust(SETTING.CLIMB_THRESH_MIN, SETTING.CLIMB_THRESH_MAX );                                  
  startActivity(CLIMB, NO_INT, SETTING.CLIMB_BEEP_TRIGGER, NO_BOOL);


    /*#######################################*/
   /////////////////SINK MENU////////////////
  /*######################################*/   
  menuItem( "SINK=-10", "+1*", "-1", " ", " ", "<-", "->" );
  itemPurpose( NAMED_FLOAT, FLOAT_ADJUSTER, FLOAT_ADJUSTER, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, THRESHOLD, MAIN_ACTIVITY );
  floatAdjust(SETTING.SINK_THRESH_MIN, SETTING.SINK_THRESH_MAX);                                  
  startActivity(SINK, NO_INT, SETTING.SINK_ALARM_TRIGGER, NO_BOOL);


    /*#######################################*/
   ////////////////PITCH MENU////////////////
  /*######################################*/     
  menuItem( "PITCH", ">CLMBmax*", ">CLMBmin", ">SINKmax", ">SINKmin", "<-", "->" );
  itemPurpose( NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, CLIMB_MAX, CLIMB_MIN, SINK_MAX, SINK_MIN, BEEP, MAIN_ACTIVITY );
  startActivity(PITCH, NO_INT, NO_FLOAT, NO_BOOL );


    /*#######################################*/
   ///////////////CLIMB_MAX MENU/////////////
  /*######################################*/     
  menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  itemPurpose( NAMED_FLOAT, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, PITCH, MAIN_ACTIVITY );
  floatAdjust(10, 4000);                                   
  startActivity(CLIMB_MAX, NO_INT, SETTING.CLIMB_PITCH_MAX, NO_BOOL );


    /*#######################################*/
   ///////////////CLIMB_MIN MENU/////////////
  /*######################################*/     
  menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  itemPurpose( NAMED_FLOAT, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, PITCH, MAIN_ACTIVITY );
  floatAdjust(10, 4000);                                   
  startActivity(CLIMB_MIN, NO_INT, SETTING.CLIMB_PITCH_MIN, NO_BOOL ); 


    /*#######################################*/
   ////////////////SINK_MAX MENU/////////////
  /*######################################*/     
  menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  itemPurpose( NAMED_FLOAT, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, PITCH, MAIN_ACTIVITY );
  floatAdjust(10, 4000);                                   
  startActivity(SINK_MAX, NO_INT, SETTING.SINK_PITCH_MAX, NO_BOOL );   


    /*#######################################*/
   ////////////////SINK_MIN MENU/////////////
  /*######################################*/     
  menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  itemPurpose( NAMED_FLOAT, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, FLOAT_ADJUSTER, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, PITCH, MAIN_ACTIVITY );
  floatAdjust(10, 4000);                                   
  startActivity(SINK_MIN, NO_INT, SETTING.SINK_PITCH_MIN, NO_BOOL );      
        

    /*#######################################*/
   ///////////////BLUETOOTH MENU/////////////
  /*######################################*/
  menuItem( 
    "MODE:",      //NONE
    " v^Speed*",  //ITEM_SWITCHER: SECOND ROW RETURNS A VALUE OF 1 FOR THE MODE BEING SWITCHED;
    "*Flyskyhy",  //ITEM_SWITCHER: THIRD ROW RETURNS A VALUE OF 2 FOR THE MODE BEING SWITCHED (AND SO ON...);
    " ",          //NONE
    " ",          //NONE
    "<-",         //ACTIVITY_SWITCHER
    "->"          //ACTIVITY_SWITCHER
  );  
  itemPurpose( NONE, ITEM_SWITCHER, ITEM_SWITCHER, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  startActivity(BLUETOOTH, SETTING.BLUETOOTH_MODE, NO_FLOAT, NO_BOOL ); 


    /*#######################################*/
   /////////////////OLED MENU////////////////
  /*######################################*/     
  menuItem( "DISP=ON", ">CHART*", " SPEED", " ", " ", "<-", "->" );
  itemPurpose( BOOL_TOGGLER, ACTIVITY_CHANGER, NONE, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, CHART_SPEED, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  booleanToggle( "DISP=ON", "DISP=OFF");
  startActivity(OLED, NO_INT, NO_FLOAT, SETTING.ENABLE_OLED );  


    /*#######################################*/
   //////////////CHART_SPEED MENU////////////
  /*######################################*/     
  menuItem( "CHART=30", "(seconds)", "+5*", "-5", " ", "<-", "->" );
  itemPurpose( NAMED_FLOAT, NONE, FLOAT_ADJUSTER, FLOAT_ADJUSTER, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  floatAdjust(0, 120);                                  
  startActivity(CHART_SPEED, NO_INT, SETTING.CHART_SPEED, NO_BOOL );


     //TODO-- DO I REALLY NEED TO ADD INTERFACE FOR USERS???
    /*#######################################*/
   //////////////////USER MENU///////////////
  /*######################################*/     
  menuItem( "USER", "*BRAEDIN*", " PAUL", " ", " ", "<-", "->" );
  itemPurpose( NONE, ITEM_SWITCHER, ITEM_SWITCHER, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  startActivity( USER, SETTING.USER, NO_FLOAT, NO_BOOL );

     //TODO-- (A LOT OF WORK IF I WANT TO IMPLEMENT THE FOLLOWING ACTIVITIES)
    /*#######################################*/
   ///////////////EDIT_NAMES MENU////////////
  /*######################################*/     
  menuItem( "THRESHOLD", ">CLIMB*", ">SINK", " ", " ", "<-", "->" );
  itemPurpose( NONE, NONE, NONE, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  //integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  startActivity(EDIT_NAMES, NO_INT, NO_FLOAT, NO_BOOL );

     //TODO--
    /*#######################################*/
   ////////////////USER_1 MENU///////////////
  /*######################################*/     
  menuItem( "USER_1", " ", " ", " ", " ", "<-", "->*" );
  itemPurpose( NONE, NONE, NONE, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  //integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  startActivity(USER_1, NO_INT, NO_FLOAT, NO_BOOL );   
    
     //TODO--
    /*#######################################*/
   ////////////////USER_2 MENU///////////////
  /*######################################*/     
  menuItem( "USER_2", " ", " ", " ", " ", "<-", "->*" );
  itemPurpose( NONE, NONE, NONE, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, CLIMB, SINK, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  //integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  startActivity(USER_2, NO_INT, NO_FLOAT, NO_BOOL );


     //TODO--
    /*#######################################*/
   ////////////////USER_3 MENU///////////////
  /*######################################*/     
  menuItem( "USER_3", " ", " ", " ", " ", "<-", "->*" );
  itemPurpose( NONE, NONE, NONE, NONE, NONE, ACTIVITY_CHANGER, ACTIVITY_CHANGER );
  ifSelectedGoTo( NONE, NONE, NONE, NONE, NONE, SETTINGS, MAIN_ACTIVITY );
  //integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  startActivity(USER_3, NO_INT, NO_FLOAT, NO_BOOL );


}

//=============================================================================================================
/*************************************************************************************************************/

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


void integerAdjust(int integerMin, int integerMax){
  INT_ADJUST[0] = integerMin;
  INT_ADJUST[1] = integerMax;
}


void floatAdjust(float floatMin, float floatMax){
  FLOAT_ADJUST[0] = floatMin;
  FLOAT_ADJUST[1] = floatMax;
}


void booleanToggle(String displayForTrue, String displayForFalse){
  BOOL_DISPLAYED[0] = displayForFalse;
  BOOL_DISPLAYED[1] = displayForTrue;
}


void startActivity(int pageNumber, int& intToAdjust, float& floatToAdjust, bool& boolToAdjust){
  Serial.print("CURRENT_PAGE=="); Serial.println(CURRENT_PAGE);
  
  enum {_MIN, _MAX};

  //INITIALIZE NAMED_INT, NAMED_FLOAT, AND BOOL_TOGGLER TO PROPER VALUE:
  for(int i=1; i<=numberOfItems; i++){    
    if(PURPOSE[i]==NAMED_INT){
      ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(intToAdjust);
    }
    if(PURPOSE[i]==NAMED_FLOAT){
      ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(floatToAdjust);
    }
    if(PURPOSE[i]==BOOL_TOGGLER){
      if(ITEM[i].endsWith("*")){
        if(boolToAdjust){ITEM[i] = BOOL_DISPLAYED[1]+"*";}
        else if(!boolToAdjust){ITEM[i] = BOOL_DISPLAYED[0]+"*";}
      }
      else{
        if(boolToAdjust){ITEM[i] = BOOL_DISPLAYED[1];}
        else if(!boolToAdjust){ITEM[i] = BOOL_DISPLAYED[0];}
      }
      
    }
    
    if(PURPOSE[i]==ITEM_SWITCHER){
      for(int j=1; j<=numberOfItems; j++){
        if(PURPOSE[j]==ITEM_SWITCHER && ITEM[j].startsWith("*")){
          ITEM[j].trim();
          ITEM[j] = " "+ITEM[j].substring(1);
        }
      }      
      ITEM[intToAdjust+1].trim();
      ITEM[intToAdjust+1] = "*"+ITEM[intToAdjust+1];
    }
    
  }

  //CHECK WHAT HAS BEEN INITIALIZED:
  for(int i=1; i<=numberOfItems; i++){Serial.println(ITEM[i]);}
  
  while(CURRENT_PAGE==pageNumber){
    /*Serial.println(SETTING.CLIMB_BEEP_TRIGGER);*/
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
        if(PURPOSE[j]==NONE || PURPOSE[j]==NAMED_INT || PURPOSE[j]==NAMED_FLOAT){j++;}
        else{ITEM[j]+='*'; break;}  //VALUE OF j IS THE LOCATION OF THE NEXT CURSORABLE ITEM;
      }

      //DEBUG:
      for(int I=1; I<=numberOfItems; I++){Serial.println(ITEM[I]);}
      
    }
    
    //IF A BUTTON IS HELD, SELECT/TOGGLE THE ITEM:
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
              intToAdjust += (ITEM[i].substring(1)).toInt();
              if(intToAdjust > INT_ADJUST[_MAX]){intToAdjust = INT_ADJUST[_MAX];} 
            }
            if(ITEM[i].startsWith("-")){
              intToAdjust -= (ITEM[i].substring(1)).toInt();
              if(intToAdjust < INT_ADJUST[_MIN]){intToAdjust = INT_ADJUST[_MIN];} 
            }            
            //UPDATE NAMED_INT VALUE:
            for(int i=1; i<=numberOfItems; i++){
              if(PURPOSE[i]==NAMED_INT){
                ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(intToAdjust);
              }
            }
          }

          if(PURPOSE[i]==FLOAT_ADJUSTER){
            if(ITEM[i].startsWith("+")){
              floatToAdjust += (ITEM[i].substring(1)).toInt();
              if(floatToAdjust > FLOAT_ADJUST[_MAX]){floatToAdjust = FLOAT_ADJUST[_MAX];} 
            }
            if(ITEM[i].startsWith("-")){
              floatToAdjust -= (ITEM[i].substring(1)).toInt();
              if(floatToAdjust < FLOAT_ADJUST[_MIN]){floatToAdjust = FLOAT_ADJUST[_MIN];} 
            }            
            //UPDATE NAMED_FLOAT VALUE:
            for(int i=1; i<=numberOfItems; i++){
              if(PURPOSE[i]==NAMED_FLOAT){
                ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(floatToAdjust);
              }
            }
          }

          if(PURPOSE[i]==BOOL_TOGGLER){
            if(ITEM[i]==BOOL_DISPLAYED[0]+"*"){
              ITEM[i]=BOOL_DISPLAYED[1]+"*";
              boolToAdjust = 1;
            }
            else{
              ITEM[i]=BOOL_DISPLAYED[0]+"*";
              boolToAdjust = 0;
            }
          }

          if(PURPOSE[i]==ITEM_SWITCHER){
            for(int j=1; j<=numberOfItems; j++){
              if(PURPOSE[j]==ITEM_SWITCHER && ITEM[j].startsWith("*")){
                ITEM[j] = " "+ITEM[j].substring(1);
              }
            }
            ITEM[i].trim();
            ITEM[i] = "*"+ITEM[i];
            intToAdjust = i-1;
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
