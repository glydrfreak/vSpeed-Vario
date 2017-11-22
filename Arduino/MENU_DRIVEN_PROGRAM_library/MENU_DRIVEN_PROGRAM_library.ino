#include "DEFAULT_SETTINGS.h"
#include "MENU.h"
#include "BUTTON.h"
#include "OLED.h"

#define BUZZER_PIN A5
#define POT_CS A4
#define POT_UD A3
#define POT_INC 6
#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define POWER_PIN                     A0    // DEVICE POWERS OFF IF THIS PIN GOES LOW;
#define BUTTON_PIN                    A1    // CONNECT THE OTHER BUTTON LEAD TO 3.3V;
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

BUTTON BUTTON;
Default SETTING;
MENU M;
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
  USER_3,
  POWER_OFF
};




void drawPage();
bool displayOn = true;
int prevVolume = SETTING.VOLUME;
void adjustVolumeTo(int volLevel);

void setup() {
  Serial.begin(BAUD_RATE);
  //while(!Serial);


  pinMode(POT_CS,OUTPUT);
  pinMode(POT_UD,OUTPUT);
  pinMode(POT_INC,OUTPUT);
  
  digitalWrite(POT_INC,HIGH);
  digitalWrite(POT_UD,LOW);
  digitalWrite(POT_CS,HIGH);

  adjustVolumeTo(SETTING.VOLUME);
  /*//RESET VOLUME TO ZERO:
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, LOW);
  for(int i = 0; i <= 100; i++){
    digitalWrite(POT_INC, LOW);
    digitalWrite(POT_INC, HIGH);
  }
  digitalWrite(POT_CS,HIGH);

  //START UP AT A DESIRED VOLUME:
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, HIGH);
  for(int i = 0; i <= SETTING.VOLUME; i++){
    digitalWrite(POT_INC, LOW);
    digitalWrite(POT_INC, HIGH);
  }
  digitalWrite(POT_CS,HIGH); 
  Serial.print("VOLUME:"); Serial.println(SETTING.VOLUME);*/

  

  pinMode(BUTTON_PIN, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);  //ONCE THIS PIN GOES LOW, THE DEVICE POWERS OFF;

  if(SETTING.ENABLE_OLED){ 
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.setFontType(0);
    oled.display();         // Display what's in the buffer (splashscreen)
    delay(1000);
  }
}



void loop() {
  
  unsigned long currentMillis = millis();

    /*#######################################*/
   ////////////////MAIN ACIVITY//////////////
  /*######################################*/
  while(M.CURRENT_PAGE==MAIN_ACTIVITY){
    currentMillis = millis();
    if(SETTING.ENABLE_OLED){
      displayOn = true;
      oled.clear(PAGE);  //Clear the screen
      oled.line(random(0,64), random(0,48), random(0,64), random(0,48));
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
    if(currentMillis>2000){BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, currentMillis);}
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      SETTING.ENABLE_BEEP=!SETTING.ENABLE_BEEP;
      if(SETTING.ENABLE_BEEP){tone(BUZZER_PIN, 400, 250);}
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
      M.CURRENT_PAGE=SETTINGS;
    }

  }


    /*#######################################*/
   /////////////////POWER OFF////////////////
  /*######################################*/ 
  while(M.CURRENT_PAGE==POWER_OFF){digitalWrite(POWER_PIN, LOW);}

  
  
    /*#######################################*/
   ////////////////SETTINGS MENU//////////////
  /*######################################*/  
  M.menuItem( "SETTINGS", ">BEEP", ">BTOOTH", ">DISPLAY", ">USER", "<-*", "OFF" );
  M.itemPurpose(M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER);
  M.ifSelectedGoTo(M.NONE, BEEP, BLUETOOTH, OLED, USER, MAIN_ACTIVITY, POWER_OFF);
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  //delay(500);
  while(M.CURRENT_PAGE==SETTINGS){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   /////////////////BEEP MENU////////////////
  /*######################################*/                                                                                            //__________________________
  M.menuItem(                                                                                                                           //  ^
    "BEEP",                                                                                                                             //  |
    ">VOLUME*",                                                                                                                         //  THIS PORTION
    ">THRESH",                                                                                                                          //  IS ACTING AS
    ">PITCH",                                                                                                                           //  void setup();  
    " ",                                                                                                                                //  |
    "<-", "->"                                                                                                                          //  |
  );                                                                                                                                    //  |
  M.itemPurpose(M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER);    //  |
  M.ifSelectedGoTo(M.NONE, VOL, THRESHOLD, PITCH, M.NONE, SETTINGS, MAIN_ACTIVITY);                                                     //  |
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );                                                                              //__v_______________________
  while(M.CURRENT_PAGE==BEEP){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}                                         //<-----THIS LINE IS ACTING AS void loop();
  


    /*#######################################*/
   ////////////////VOLUME MENU///////////////
  /*######################################*/  
  M.menuItem(//NAMED_INT AND M.NAMED_FLOAT, AND M.BOOL_TOGGLER WILL AUTOMATICALLY UPDATE TO THEIR PROPER VALUE;
    "VOL=THE_VALUE_AFTER_THE_EQUAL_SIGN_WILL_RE-INITIALIZE_AUTOMATICALLY",
    "+1*",
    "-1",
    "THIS_ITEM_WILL_RE-INITIALIZE_ACCORDING_TO_THE_PARAMETER_IN M.booleanToggle();",
    " ",
    "<-", "->"
  );
  M.itemPurpose(M.NAMED_INT, M.INT_ADJUSTER, M.INT_ADJUSTER, M.BOOL_TOGGLER, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER);   //POSSIBLE PURPOSES WHEN CORRESPONDING ITEM IS SELECTED;
  M.ifSelectedGoTo(M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, BEEP, MAIN_ACTIVITY);                                                //POSSIBLE NEXT PAGES WHEN CORRESPONDING ITEM IS SELECTED;
  M.integerAdjust(0, 4);                                                                                                        //PARAMETERS TO ADJUST THE NAMED_INT VALUE DISPLAYED;
  M.booleanToggle( "(ON)", "(MUTED)" );                                                                                         //BOOLEAN VARIABLE TO SWAP BETWEEN TRUE AND FALSE;
  M.initializeActivity( SETTING.VOLUME, M.NO_FLOAT, SETTING.ENABLE_BEEP );                                                      //INITIALIZE THE ACTIVITY;
  /*int steps = 0;
  int adjustedFrom = SETTING.VOLUMES[SETTING.VOLUME];
  bool turnedMuteOn = false;*/
  while(M.CURRENT_PAGE==VOL){M.launchActivity( SETTING.VOLUME, M.NO_FLOAT, SETTING.ENABLE_BEEP ); drawPage();                   //START THE ACTIVITY; drawPage() IS CREATED BY USER;
    adjustVolumeTo(SETTING.VOLUME);
    /*if(!SETTING.ENABLE_BEEP){
      SETTING.VOLUME = 0;
      turnedMuteOn = true;
    }
    if(turnedMuteOn && SETTING.ENABLE_BEEP){
      turnedMuteOn = false;
      SETTING.VOLUME = prevVolume;
    }*/
    /*if(SETTING.VOLUME > adjustedFrom){                                                                                          //ADD ANY ADDITIONAL CODE IF NEEDED;
      steps = SETTING.VOLUME - adjustedFrom;
      adjustedFrom = SETTING.VOLUME;
      if(!turnedMuteOn){prevVolume = SETTING.VOLUME;}
      digitalWrite(POT_CS,LOW);
      digitalWrite(POT_UD, HIGH);
      for(int i = 0; i < steps; i++){
        digitalWrite(POT_INC, LOW);
        digitalWrite(POT_INC, HIGH);
      }
      Serial.print("VOLUME_UP_TO:"); Serial.println(SETTING.VOLUME);
      digitalWrite(POT_CS ,HIGH);  
    }
    else{
      steps = adjustedFrom - SETTING.VOLUME;
      adjustedFrom = SETTING.VOLUME;
      if(!turnedMuteOn){prevVolume = SETTING.VOLUME;}
      digitalWrite(POT_CS,LOW);
      digitalWrite(POT_UD, LOW);
      for(int i = 0; i < steps; i++){
        digitalWrite(POT_INC, LOW);
        digitalWrite(POT_INC, HIGH);
      }
      Serial.print("VOLUME_DOWN_TO:"); Serial.println(SETTING.VOLUME);
      digitalWrite(POT_CS ,HIGH);      
    }*/
    
    tone(BUZZER_PIN, 400, 100);
  }                  
  //STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);
  //STORAGE.storeVariable(STORAGE.search_ENABLE_BEEP, SETTING.ENABLE_BEEP);



    /*#######################################*/
   ///////////////THRESHOLD MENU/////////////
  /*######################################*/   

  //LIST OF ITEMS TO DISPLAY IN THIS MENU ACTIVITY:
  M.menuItem(
    "THRESHOLD",        //RECOMMENDED: NINE OR LESS CHARACTERS LONG, INCLUDING CURSOR IF THE ITEM IS CURSORABLE;
    ">CLIMB*",          //RECOMMENDED: INITIAL CURSOR "*" PLACEMENT;
    ">SINK",
    " ",
    " ",
    "<-",               //RECOMMENDED: "<-";
    "->"                //RECOMMENDED: "->";
  );
  
  //PURPOSES: NONE, M.ACTIVITY_CHANGER, NAMED_INT, INT_ADJUSTER, M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.BOOL_TOGGLER; //TODO-- M.ITEM_SWITCHER;
  M.itemPurpose(
    M.NONE,               //RECOMMENDED: M.NONE, NAMED_INT, OR M.NAMED_FLOAT;
    M.ACTIVITY_CHANGER, 
    M.ACTIVITY_CHANGER, 
    M.NONE, 
    M.NONE, 
    M.ACTIVITY_CHANGER,   //RECOMMENDED: M.ACTIVITY_CHANGER;
    M.ACTIVITY_CHANGER    //RECOMMENDED: M.ACTIVITY_CHANGER;
  );
  
  //ACTIVITIES: M.NONE, OR ANY ACTIVITY DEFINED;
  M.ifSelectedGoTo(
    M.NONE,               //RECOMMENDED: M.NONE;
    CLIMB, 
    SINK, 
    M.NONE, 
    M.NONE, 
    BEEP,               //RECOMMENDED: NAME OF THE PREVIOUS ACTIVITY;
    MAIN_ACTIVITY       //RECOMMENDED: MAIN_ACTIVITY;
  );
  
  //M.integerAdjust(SETTING_MIN, SETTING_MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING_MIN, SETTING_MAX);    //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "TRUE", "FALSE" );         //COMMENT THIS LINE IF UNUSED;

  //ACTIVITY START, AND REFERENCED VARIABLES TO ADJUST IF APPLICABLE:
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==THRESHOLD){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}



    /*#######################################*/
   ////////////////CLIMB MENU////////////////
  /*######################################*/    
  M.menuItem( "CLIMB=1", "+1*", "-1", " ", " ", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, THRESHOLD, MAIN_ACTIVITY );
  M.floatAdjust(SETTING.CLIMB_THRESH_MIN, SETTING.CLIMB_THRESH_MAX );                                  
  M.initializeActivity( M.NO_INT, SETTING.CLIMB_BEEP_TRIGGER, M.NO_BOOL );
  while(M.CURRENT_PAGE==CLIMB){M.launchActivity( M.NO_INT, SETTING.CLIMB_BEEP_TRIGGER, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   /////////////////SINK MENU////////////////
  /*######################################*/   
  M.menuItem( "SINK=-10", "+1*", "-1", " ", " ", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, THRESHOLD, MAIN_ACTIVITY );
  M.floatAdjust(SETTING.SINK_THRESH_MIN, SETTING.SINK_THRESH_MAX);                                  
  M.initializeActivity( M.NO_INT, SETTING.SINK_ALARM_TRIGGER, M.NO_BOOL );
  while(M.CURRENT_PAGE==SINK){M.launchActivity( M.NO_INT, SETTING.SINK_ALARM_TRIGGER, M.NO_BOOL ); drawPage();}

    /*#######################################*/
   ////////////////PITCH MENU////////////////
  /*######################################*/     
  M.menuItem( "PITCH", ">CLMBmax*", ">CLMBmin", ">SINKmax", ">SINKmin", "<-", "->" );
  M.itemPurpose( M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CLIMB_MAX, CLIMB_MIN, SINK_MAX, SINK_MIN, BEEP, MAIN_ACTIVITY );
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==PITCH){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ///////////////CLIMB_MAX MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.CLIMB_PITCH_MAX, M.NO_BOOL );
  while(M.CURRENT_PAGE==CLIMB_MAX){M.launchActivity( M.NO_INT, SETTING.CLIMB_PITCH_MAX, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ///////////////CLIMB_MIN MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.CLIMB_PITCH_MIN, M.NO_BOOL ); 
  while(M.CURRENT_PAGE==CLIMB_MIN){M.launchActivity( M.NO_INT, SETTING.CLIMB_PITCH_MIN, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ////////////////SINK_MAX MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.SINK_PITCH_MAX, M.NO_BOOL );
  while(M.CURRENT_PAGE==SINK_MAX){M.launchActivity( M.NO_INT, SETTING.SINK_PITCH_MAX, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ////////////////SINK_MIN MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.SINK_PITCH_MIN, M.NO_BOOL );
  while(M.CURRENT_PAGE==SINK_MIN){M.launchActivity( M.NO_INT, SETTING.SINK_PITCH_MIN, M.NO_BOOL ); drawPage();} 
        

    /*#######################################*/
   ///////////////BLUETOOTH MENU/////////////
  /*######################################*/
  M.menuItem( 
    "MODE:",      //M.NONE
    " v^Speed*",  //ITEM_SWITCHER: SECOND ROW RETURNS A VALUE OF 1 FOR THE MODE BEING SWITCHED;
    "*Flyskyhy",  //ITEM_SWITCHER: THIRD ROW RETURNS A VALUE OF 2 FOR THE MODE BEING SWITCHED (AND SO ON...);
    " ",          //M.NONE
    " ",          //M.NONE
    "<-",         //ACTIVITY_SWITCHER
    "->"          //ACTIVITY_SWITCHER
  );  
  M.itemPurpose( M.NONE, M.ITEM_SWITCHER, M.ITEM_SWITCHER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  M.initializeActivity( SETTING.BLUETOOTH_MODE, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==BLUETOOTH){M.launchActivity( SETTING.BLUETOOTH_MODE, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   /////////////////OLED MENU////////////////
  /*######################################*/     
  M.menuItem( "DISP=ON", ">CHART*", " SPEED", " ", " ", "<-", "->" );
  M.itemPurpose( M.BOOL_TOGGLER, M.ACTIVITY_CHANGER, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CHART_SPEED, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  M.booleanToggle( "DISP=ON", "DISP=OFF");
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, SETTING.ENABLE_OLED );  
  while(M.CURRENT_PAGE==OLED){M.launchActivity( M.NO_INT, M.NO_FLOAT, SETTING.ENABLE_OLED ); drawPage();}


    /*#######################################*/
   //////////////CHART_SPEED MENU////////////
  /*######################################*/     
  M.menuItem( "CHART=30", "(seconds)", "+5*", "-5", " ", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.NONE, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  M.floatAdjust(0, 120);                                  
  M.initializeActivity( M.NO_INT, SETTING.CHART_SPEED, M.NO_BOOL /*,NO_STRING*/ );
  while(M.CURRENT_PAGE==CHART_SPEED){M.launchActivity( M.NO_INT, SETTING.CHART_SPEED, M.NO_BOOL ); drawPage();}


     //TODO-- DO I REALLY NEED TO ADD INTERFACE FOR USERS???
    /*#######################################*/
   //////////////////USER MENU///////////////
  /*######################################*/     
  M.menuItem( "USER", "*BRAEDIN*", " PAUL", " ", " ", "<-", "->" );
  M.itemPurpose( M.NONE, M.ITEM_SWITCHER, M.ITEM_SWITCHER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  M.initializeActivity( SETTING.USER, M.NO_FLOAT, M.NO_BOOL /*, SETTING.USER_NAME_1*/ );
  while(M.CURRENT_PAGE==USER){M.launchActivity( SETTING.USER, M.NO_FLOAT, M.NO_BOOL ); drawPage();}

  

     //TODO-- (A LOT OF WORK IF I WANT TO IMPLEMENT THE FOLLOWING ACTIVITIES)
    /*#######################################*/
   ///////////////EDIT_NAMES MENU////////////
  /*######################################*/     
  M.menuItem( "THRESHOLD", ">CLIMB*", ">SINK", " ", " ", "<-", "->" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==EDIT_NAMES){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}
  

     //TODO--
    /*#######################################*/
   ////////////////USER_1 MENU///////////////
  /*######################################*/     
  M.menuItem( "USER_1", " ", " ", " ", " ", "<-", "->*" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); 
  while(M.CURRENT_PAGE==USER_1){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}

    
    
     //TODO--
    /*#######################################*/
   ////////////////USER_2 MENU///////////////
  /*######################################*/     
  M.menuItem( "USER_2", " ", " ", " ", " ", "<-", "->*" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CLIMB, SINK, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==USER_2){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


     //TODO--
    /*#######################################*/
   ////////////////USER_3 MENU///////////////
  /*######################################*/     
  M.menuItem( "USER_3", " ", " ", " ", " ", "<-", "->*" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==USER_3){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


}

//=============================================================================================================
/*************************************************************************************************************/




void drawPage(){
  oled.clear(PAGE);
  oled.setCursor(0,0);
  for(int i=1; i<=M.numberOfItems; i++){
    if(i<=M.numberOfItems-2){oled.println(M.ITEM[i]);}
    else{
      oled.setCursor(0,41); oled.print(M.ITEM[6]);
      oled.setCursor(36,41); oled.print(M.ITEM[7]);
    }
  }
  oled.display();
}



void adjustVolumeTo(int volLevel){
  //RESET VOLUME TO ZERO:
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, LOW);
  for(int i = 0; i <= 100; i++){
    digitalWrite(POT_INC, LOW);
    digitalWrite(POT_INC, HIGH);
  }
  digitalWrite(POT_CS,HIGH);

  //MOVE UP TO A DESIRED VOLUME:
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, HIGH);
  for(int i = 0; i <= SETTING.VOLUMES[volLevel]; i++){
    digitalWrite(POT_INC, LOW);
    digitalWrite(POT_INC, HIGH);
  }
  digitalWrite(POT_CS,HIGH); 
  Serial.print("VOLUME:"); Serial.println(SETTING.VOLUMES[volLevel]);
}

