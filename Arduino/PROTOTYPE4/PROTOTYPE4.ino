//(in progress)

/*
 * NOTES:
 *  EPAPER TAKES SIGIFICANT TIME TO UPDATE EACH FRAME;
 *  WHEN THE DISPLAY IS ENABLED, THE ALTIDUDE DATA IS VERY NOISY AT 87 SAMPLES PER SECOND;
 *  WHEN THE DISPLAY IS OFF, THE ALTITUDE DATA IS VERY SMOOTH AT 91 SAMPLES PER SECOND;
 */
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                       v^SPEED VARIO -- PROTOTYPE[4]                          //
//                        (Vertical Speed Indicator)                            //
/*           Writen by Braedin Butler, with many other contributers             */
//BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|

#include "DEFAULT_SETTINGS.h"
#include "MENU.h"
#include "BUTTON.h"
#include "BEEP.h"
#include "MICRO_SD.h"
#include "vAdafruit_BluefruitLE_SPI.h"
#include "MS5611.h"
#include "FILTER.h"
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1
#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define POWER_PIN                     A0    // DEVICE POWERS OFF IF THIS PIN GOES LOW;
#define BUTTON_PIN                    A1    // CONNECT THE OTHER BUTTON LEAD TO 3.3V;
#define POT_CS                        A3
#define SD_PIN                        A2
#define MS5611_CSB                    13    // Chip/Slave Select Pin
#define D1_OSR                         5    // (Default pressure OSR mode 5) 
#define D2_OSR                         2    // (Default temperature OSR mode 2) 
#define VELOS_AVGERAGED               30    // Number of most recent velocity values averaged; max is maxVeloData set in MS5611.h
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer 
#define VBATPIN                        9    // Pin monitors battery level (Pin A7)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended set 4, set to -1 if unused...
#define VERBOSE_MODE               false    // If set to 'true' enables debug output

BUTTON BUTTON;
Default SETTING;
MENU M;
MEMORY STORAGE;
MS5611_SPI MS5611;
FILTER FILTER;
BEEP BEEP;
vAdafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;


enum {  //POSSIBLE ACTIVITY NAMES:
  MAIN_ACTIVITY, SETTINGS, BEEP_MENU, VOL, THRESHOLD,
  CLIMB, SINK, PITCH, CLIMB_MAX, CLIMB_MIN, SINK_MAX,
  SINK_MIN, BLUETOOTH, OLED, CHART_SPEED, 
  USER, EDIT_NAMES, USER_1, USER_2, USER_3,
  POWER_OFF
};


void SWITCH_BLE_MODE(int bluetoothMode);    // BLE MODULE INITIALIZES ACCORDING TO SPECIFIED BLUETOOTH_MODE;
void receiveCommands();
void transmitVspeed();
void transmitFlySkyHy();

void PrintAltitude(int number);
void SimplerVelocityNeedle(float number);
void displayVario(int number);
void EPAPER_MAIN(float altiFt, float veelawcity);
int displayLoop = 0;

void drawPage();
bool displayOn = true;
int prevVolume = SETTING.VOLUME;
void adjustVolumeTo(int volLevel);
float getBatteryLvl();

void scrollingAltitude(float scrolledAlti); // Custom OLED Widget
void liveChart(int v, float speed);         // Custom OLED Widget
void batteryIcon(float battLvl);            // Custom OLED Widget

float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
int AVERAGING_DURATION = 1000;    // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long displayMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
long battMillis = -5000;
float batteryLvl = 0;
int batteryPercent = 100;



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                                    SETUP                                     //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
void setup() {
  Serial.begin(BAUD_RATE);
  //while(!Serial);

  pinMode(VBATPIN, INPUT);

  pinMode(POT_CS,OUTPUT);
  

  pinMode(BUTTON_PIN, INPUT);
  
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);  //ONCE THIS PIN GOES LOW, THE DEVICE POWERS OFF;

  //if(SD.begin(SD_PIN)){ //CHECK IF SD IS INSERTED, ESPECIALLY TO UPDATE DEFAULT PREFERENCES:
  //  SETTING.BLUETOOTH_MODE = STORAGE.findVariable(STORAGE.search_BLUETOOTH_MODE);
  //  //TODO-- ...
  //}
  
  if(SETTING.ENABLE_BLUETOOTH){
    ble.begin(VERBOSE_MODE);
    SWITCH_BLE_MODE(SETTING.BLUETOOTH_MODE);  //INITIALIZES THE SPECIFIED BLUETOOTH MODE; 
  }
  
  MS5611.begin(MS5611_CSB);

  BEEP.begin(BEEP_PIN);
  BEEP.setClimbThreshold(SETTING.CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SETTING.SINK_ALARM_TRIGGER);   //ft/s
  BEEP.setClimbPitchMax(SETTING.CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(SETTING.CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SETTING.SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SETTING.SINK_PITCH_MIN);             //Hz

  //INITIALIZE FULL UPDATE MODE AT START UP TO COMPLETELY ERASE THE SCREEN: 
  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
  
  //GET RID OF ANY GHOST IMAGE THAT REMAINS BY FLICKERING THE ENTIRE SCREEN:
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  
  //INITIALIZE PARTIAL UPDATE MODE AFTER GHOST IMAGES ARE CLEARED:
  if (epd.Init(lut_partial_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  adjustVolumeTo(SETTING.VOLUMES[SETTING.VOLUME]);
  
} /*end setup*/



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                                     LOOP                                     //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
void loop() {
  
  currentMillis = millis();

    /*#######################################*/
   ////////////////MAIN ACIVITY//////////////
  /*######################################*/
  while(M.CURRENT_PAGE==MAIN_ACTIVITY){
    currentMillis = millis();
    samplesThisSec++; //increment each time the loop cycles
    if(currentMillis - previousMillis >= 1000){ // Update value of samplesPerSec once every second:
      previousMillis=currentMillis;
      samplesPerSec = samplesThisSec;
      samplesThisSec=0; 
      //Serial.println(samplesPerSec);  //print debug info
    }
    
    /*if(SETTING.ENABLE_OLED){
      displayOn = true;
      oled.clear(PAGE);  //Clear the screen*/
    
      //====BATTERY================================================================/
        //Measure battery level every 5 seconds
        if(SETTING.MEASURE_BATTERY && currentMillis-battMillis>=5000){
          battMillis = currentMillis;
          batteryLvl = getBatteryLvl();
          batteryPercent = (int)(batteryLvl*156.25 - 556.25);
          if(batteryPercent > 99){batteryPercent = 99;}
          else if(batteryPercent < 0){batteryPercent = 0;}
        }
    
      //====MS5611=================================================================/
        temperatureF = MS5611.getTemperatureF(D2_OSR);                                                                  //TEMPERATURE
        pressurePa = MS5611.getPressurePa(D1_OSR);                                                                      //PRESSURE
        altitudeFt = MS5611.getAltitudeFt(temperatureF, pressurePa);                                                    //ALTITUDE
        altitudeFt = FILTER.RUNNING_AVERAGE(altitudeFt, samplesPerSec, AVERAGING_DURATION);                             //FILTER
        velocityFtPerSec = MS5611.getVelocityFtPerSec(altitudeFt, currentMillis, VELOS_AVGERAGED);                      //VERTICAL SPEED      
        Serial.print(altitudeFt); 
        //Serial.print(" "); 
        //Serial.print(velocityFtPerSec);
        Serial.println();
        //TODO--OBTAIN STARTUP VOLUME FROM MICRO SD;
        if(SETTING.ENABLE_BEEP && currentMillis > 6000){                                                                //BEEP
          if(SETTING.BEEP_TYPE == 1){BEEP.basedOnVelocity(altitudeFt, velocityFtPerSec, currentMillis);}
          if(SETTING.BEEP_TYPE == 2){BEEP.bufferedDurationIncrements(altitudeFt, velocityFtPerSec, currentMillis);}
        }
    
      //====BLE====================================================================/ 
        if(SETTING.ENABLE_BLUETOOTH){
          if(SETTING.BLUETOOTH_MODE==1){
            receiveCommands();                //Custom Bluetooth Commands Handled When Received From Mobile Devices
            transmitVspeed();                 //Custom Bluetooth Transmissions to Mobile Devices
          }
          else if(SETTING.BLUETOOTH_MODE==2){
            //TODO-- RECIEVE VOLUME COMMANDS FROM FLYSKYHY;
            transmitFlySkyHy();
          }
        }
        
      //====EPD====================================================================/
        //WAIT FOR A CERTAIN NUMBER OF LOOPS BEFORE UPDATING THE EPAPER DISPLAY:
        /*displayLoop++; */
        if(SETTING.ENABLE_OLED && currentMillis-displayMillis>=1000){
          /*displayLoop=0;*/
          displayMillis = currentMillis;
          EPAPER_MAIN(altitudeFt, velocityFtPerSec);
        }
        
   
//        if(/*SETTING.ENABLE_OLED &&*/ currentMillis>2000){
//          liveChart(velocityFtPerSec, SETTING.CHART_SPEED);
//          if(SETTING.MEASURE_BATTERY){batteryIcon(batteryLvl);}
//          scrollingAltitude(altitudeFt);
//          if(velocityFtPerSec>=0){/*oled.setCursor(50,41);*/}
//          else{/*oled.setCursor(44,41);*/}
//          /*oled.print(velocityFtPerSec,0);
//          oled.display();   //Draw the new screen*/
//        }
//
//      
//      /*oled.display();   //Draw the new screen
//    }
//    else if(displayOn){
//      displayOn = false;
//      oled.clear(PAGE);
//      oled.display();
//    }*/

//    if(currentMillis>2000){BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, currentMillis);}
//    if(BUTTON.PRESS==BUTTON.CLICK){
//      BUTTON.PRESS=BUTTON.NO_ACTION; 
//      SETTING.ENABLE_BEEP=!SETTING.ENABLE_BEEP;
//      if(SETTING.ENABLE_BEEP){tone(BEEP_PIN, 400, 250);}
//      /*oled.clear(PAGE);
//      oled.setCursor(0,0);*/
//      if(!SETTING.ENABLE_BEEP){/*oled.print("BEEP=OFF");*/}
//      else{/*oled.print("BEEP=ON");*/}
//      /*oled.display();*/
//      delay(250);
//      /*oled.clear(PAGE);
//      oled.display();*/
//    }
//    if(BUTTON.PRESS==BUTTON.HOLD){
//      BUTTON.PRESS=BUTTON.NO_ACTION; 
//      M.CURRENT_PAGE=SETTINGS;
//    }

      //Serial.println(M.CURRENT_PAGE);

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
  M.ifSelectedGoTo(M.NONE, BEEP_MENU, BLUETOOTH, OLED, USER, MAIN_ACTIVITY, POWER_OFF);
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
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
  while(M.CURRENT_PAGE==BEEP_MENU){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}                                         //<-----THIS LINE IS ACTING AS void loop();
  


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
  M.ifSelectedGoTo(M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, BEEP_MENU, MAIN_ACTIVITY);                                                //POSSIBLE NEXT PAGES WHEN CORRESPONDING ITEM IS SELECTED;
  M.integerAdjust(0, 4);                                                                                                        //PARAMETERS TO ADJUST THE NAMED_INT VALUE DISPLAYED;
  M.booleanToggle( "(ON)", "(MUTED)" );                                                                                         //BOOLEAN VARIABLE TO SWAP BETWEEN TRUE AND FALSE;
  M.initializeActivity( SETTING.VOLUME, M.NO_FLOAT, SETTING.ENABLE_BEEP );                                                      //INITIALIZE THE ACTIVITY;
  while(M.CURRENT_PAGE==VOL){M.launchActivity( SETTING.VOLUME, M.NO_FLOAT, SETTING.ENABLE_BEEP ); drawPage();                   //START THE ACTIVITY; drawPage() IS CREATED BY USER;
    adjustVolumeTo(SETTING.VOLUME);
    tone(BEEP_PIN, 400, 100);
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
    BEEP_MENU,               //RECOMMENDED: NAME OF THE PREVIOUS ACTIVITY;
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
  M.ifSelectedGoTo( M.NONE, CLIMB_MAX, CLIMB_MIN, SINK_MAX, SINK_MIN, BEEP_MENU, MAIN_ACTIVITY );
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


} /*end loop*/

//=============================================================================================================
/*************************************************************************************************************/



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                                  BATTERY ICON                                //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void batteryIcon(float battLvl){
  int bars = round((((3 - 0) / (4.2 - 3.5)) * (battLvl - 3.5)) + 0);
  /*oled.rect(57,0, 6,4);
  oled.rect(58,1, 1+bars,2);
  oled.pixel(63,1);
  oled.pixel(63,2);*/
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                           TO DRAW EACH MENU ITEM                             //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void drawPage(){
  /*oled.clear(PAGE);
  oled.setCursor(0,0);*/
  for(int i=1; i<=M.numberOfItems; i++){
    if(i<=M.numberOfItems-2){
      /*oled.println(M.ITEM[i]);*/
      paint.SetRotate(ROTATE_180);
      paint.SetWidth(128);
      paint.SetHeight(28);
      paint.Clear(UNCOLORED);
      //paint.DrawStringAt(0,4, M.ITEM[i].toCharArray(), &Font24, COLORED);
      epd.SetFrameMemory(paint.GetImage(), 0, 296-28 - i*28, paint.GetWidth(), paint.GetHeight());
    }
    else{
      /*oled.setCursor(0,41); oled.print(M.ITEM[6]);
      oled.setCursor(36,41); oled.print(M.ITEM[7]);*/
      paint.SetRotate(ROTATE_180);
      paint.SetWidth(64);
      paint.SetHeight(28);
      paint.Clear(UNCOLORED);
      //paint.DrawStringAt(0,4, M.ITEM[6].toCharArray(), &Font24, COLORED);
      epd.SetFrameMemory(paint.GetImage(), 0, 296-28 - 6*28, paint.GetWidth(), paint.GetHeight());

      paint.SetRotate(ROTATE_180);
      paint.SetWidth(64);
      paint.SetHeight(28);
      paint.Clear(UNCOLORED);
      //paint.DrawStringAt(0,4, M.ITEM[7].toCharArray(), &Font24, COLORED);
      //paint.DrawCharAt(0,4, M.ITEM[7], &Font24, COLORED);
      epd.SetFrameMemory(paint.GetImage(), 64, 296-28 - 6*28, paint.GetWidth(), paint.GetHeight());      
    }
  }
  /*oled.display();*/
  epd.DisplayFrame();
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                            DIGITAL VOLUME CONTROL                            //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void adjustVolumeTo(int volLevel){
  
  // take the SS pin low to select the chip:
  digitalWrite(POT_CS, LOW);
  
  //  send in the address and value via SPI:
  int address = 0;
  SPI.transfer(address);
  SPI.transfer(volLevel);
  
  // take the SS pin high to de-select the chip:
  digitalWrite(POT_CS, HIGH);
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                        TO MEASURE BATTERY LEVEL                             //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
float getBatteryLvl(){  
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                    LIVE CHART WITH CUSTOM SCROLLING SPEED                    //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
int y[64] = {24};
unsigned long chartMillis = 0;
void liveChart(int v, float speed){
  
  //IF ITS TIME TO SHIFT THE LINE CHART
  if(currentMillis-chartMillis >= speed*1000.0/64.0 ){
    chartMillis=currentMillis;
    int p = -2*(v) + 24;
    for(int i = 0; i < 63; i++){
      y[i] = y[i+1];        // Shift all pixels to the left one
      /*oled.line(i,y[i], i+1,y[i+1]);*/
    }

    y[63] = p;
    if(y[63]>47){y[63]=47;}
    else if(y[63]<0){y[63]=0;}
  
    /*oled.line(62,y[62], 63,y[63]);*/

  }

  //OTHERWISE, EVERYTHING STAYS THE SAME
  else{ for(int i = 0; i < 63; i++){/*oled.line(i,y[i], i+1,y[i+1]);*/} }
    
  //DRAW THE ZERO AXIS
  for(int i = 3; i < 60; i+=4){ /*oled.pixel(i, 24);*/ }
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                           SCROLLING ALTITUDE WIDGET                          //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void scrollingAltitude(float scrolledAlti){
  int Position = (((scrolledAlti - (int)scrolledAlti)*50));  //Calculate scrolledAlti's position
  /*oled.setCursor(2,Position-24);oled.print(scrolledAlti,0);
  oled.setCursor(2,Position+24);oled.print(scrolledAlti-1,0);
  oled.rect(0,22,27,11);
  oled.rectFill(1, 23, 25, 9, 0, 0);
  oled.setCursor(2,24);
  oled.print(scrolledAlti,0);*/
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//       Custom Bluetooth Commands Handled When Received From Mobile Devices;   //
//        CURRENTLY, NO BLUETOOTH COMMANDS CHANGE ANY STORED DEFAULT VALUES;    //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void receiveCommands(){
  // Check for incoming characters from Mobile Device
  ble.println("AT+BLEUARTRX");
  ble.readline(); //I think this receives something from somewhere and puts it into some buffer..?
  
  if (strcmp(ble.buffer, "OK")) {
    //Serial.print("    [Something is in the buffer]:[");
    String command = ble.buffer;
    //Serial.print(command);Serial.println("]     ");

    /*if(command == "m"){ENABLE_MS5611 = false;}
    else if(command == "M"){ENABLE_MS5611 = true;}*/

    if(command == "k"){SETTING.ENABLE_BLUETOOTH = 0;}  // Kill BLE connection
    
    if(command == "V"){SETTING.MEASURE_BATTERY=1;}        // display supply power supply voltage
    else if(command == "v"){SETTING.MEASURE_BATTERY=0;}  // display "0.00V" and don't calculate anything to improve samplesPerSec

    if(command.startsWith("a")){
      // Example: "a250"
      /*if(command == "a" || command == "a0"){ENABLE_FILTER = false;}*/
      /*else{*/
        String s = command.substring(1);
        float f = s.toFloat();
        if(f>1000){f=1000.0;}
        if(f<1){f=1; /*ENABLE_FILTER = false;*/}
        AVERAGING_DURATION = f; 
        /*ENABLE_FILTER = true;*/
      /*}*/
    }


    if(command.startsWith("ct")){
      // Example: "ct2"
      String s = command.substring(2);
      float f = s.toFloat();
      if(f<1){f=1;}
      BEEP.setClimbThreshold(f);       //ft climbed
    }
    
    if(command.startsWith("st")){
      // Example: "st-8"
      String s = command.substring(2);
      float f = s.toFloat();
      BEEP.setSinkAlarmThreshold(f);   //ft/s
    }

    if(command.startsWith("cpx")){
      // Example: "cpx800"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setClimbPitchMax(f);        //Hz
      for(int i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);       
    }

    if(command.startsWith("cpn")){
      // Example: "cpn300"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setClimbPitchMin(f);        //Hz
      for(int i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }

    if(command.startsWith("spx")){
      // Example: "cpx250"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setSinkPitchMax(f);        //Hz
      for(int i = BEEP.sinkPitchMin; i <= BEEP.sinkPitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.sinkPitchMax; i >= BEEP.sinkPitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }

    if(command.startsWith("spn")){
      // Example: "cpn200"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setSinkPitchMin(f);        //Hz
      for(int i = BEEP.sinkPitchMin; i <= BEEP.sinkPitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.sinkPitchMax; i >= BEEP.sinkPitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);
    }    

   
    if(command == "B"){  // TURN BEEP ON
      SETTING.ENABLE_BEEP = 1;
      //Serial.println("  beep:ON");
      for(float i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }
    else if(command == "b"){  // TURN BEEP OFF
      for(float i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);
      //Serial.println("  beep:OFF");
      SETTING.ENABLE_BEEP = 0;       
    }      
    
    
    if(command == "d"){
      /*oled.clear(ALL);        // Clear the display's internal memory
      SETTING.ENABLE_OLED = 0;
      //Serial.println("  oled:OFF");*/
    }
    else if(command == "D"){
      /*//Serial.println("  oled:ON");
      SETTING.ENABLE_OLED = 1;    
      oled.clear(ALL);        // Clear the display's internal memory
      oled.drawBitmap(oled.logo);  // Draw v^SPEED logo
      oled.display();         // Display what's in the buffer (splashscreen)*/
      delay(1000);
    }


    /*if(command.startsWith("w")){
      // Example: "w20" (ms per loop)
      String s = command.substring(1);
      float f = s.toFloat();
      dly = (int)f;
    }*/


    /*if(command.startsWith("op")){
      // Example: "op5" (pressure OSR mode 1-5)
      String s = command.substring(2);
      float f = s.toFloat();
      D1_OSR = (int)f;
      if(D1_OSR>5){D1_OSR=5;}
      else if(D1_OSR<1){D1_OSR=1;}        
    }
    if(command.startsWith("ot")){
      // Example: "ot5" (temperature OSR mode 1-5)
      String s = command.substring(2);
      float f = s.toFloat();
      D2_OSR = (int)f;
      if(D2_OSR>5){D2_OSR=5;}
      else if(D2_OSR<1){D2_OSR=1;}
    }*/

  }
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//         Custom Bluetooth Transmissions to v^Speed Vario Android App:         //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void transmitVspeed(){
  ble.print("AT+BLEUARTTX=");
  ble.print(altitudeFt);
  ble.print("_");     
  ble.print(velocityFtPerSec);  
  ble.print("_");
  if(SETTING.MEASURE_BATTERY){ble.print(batteryLvl);}
  else{ble.print("0");}
  ble.println("V");  //Critical char used for transmission completion indication
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//               Custom Bluetooth Transmissions to FlySkyHy iOS App:            //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void transmitFlySkyHy(){

  //EXAMPLE TRANSMISSION SENTENCE:
  //$LK8EX1,98684,99999,-4,28,1100,*02<CR><LF>
 
  //   where:
  //   $LK8EX1 is keyword
  //   98684 is filtered pressure in Pa relative to QNH1
  //   99999 should be altitude relative to QNH but it is ignored when pressure is available
  //   -4 is filtered vario in cm / s
  //   28 is temperature in °C
  //   1100 is battery percentage + 1000 (or 999 during charging)
  //   *02 is nmea checksum
  //   <CR><LF> CR and LF characters to terminate the line
  
  int cmPerSec = velocityFtPerSec*30.48;
  
  ble.print("AT+GATTCHAR=1,$LK8EX1,");
  ble.println(pressurePa,0);
  ble.print("AT+GATTCHAR=1,,99999,");
  ble.println(cmPerSec);
  ble.print("AT+GATTCHAR=1,,28,10");
  ble.println(batteryPercent);
  ble.print("AT+GATTCHAR=1,");
  ble.println(",*02<CR><LF>");
  
}


//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//           HANDLES THE STARTUP INITIALIZATION OF THE BLUETOOTH MODULE         //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void SWITCH_BLE_MODE(int bluetoothMode){
  
  //TO ENABLE FLYSKYHY TRANSMISSION MODE:
  if(bluetoothMode==2){

    //TO PERFORM FACTORY RESET:
    if( !ble.factoryReset() ){ Serial.println(" COULD NOT FACTORY RESET "); while(1); }
  
    //ADD NECESSARY ADVERTISING DATA:
    ble.sendCommandCheckOK(F( "AT+GATTCLEAR" )); 
    ble.sendCommandCheckOK(F( "AT+GAPSETADVDATA=02-01-06-11-07-1B-C5-D5-A5-02-00-03-A9-E3-11-8B-AA-A0-C6-79-E0" )); 

    //ADD A BLUETOOTH SERVICE AND CHARACTERISTIC FOR FLYSKYHY COMPATIBILITY: (Same specs as SkyDrop Vario)
    ble.sendCommandCheckOK(F( "AT+GATTADDSERVICE=UUID128=E0-79-C6-A0-AA-8B-11-E3-A9-03-00-02-A5-D5-C5-1B" )); 
    ble.sendCommandCheckOK(F( "AT+GATTADDCHAR=UUID=0xFFE1,PROPERTIES=0x10,MIN_LEN=7,MAX_LEN=20,VALUE=$LK8EX1" )); 
    
    //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
    ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=SkyDrop" )); ble.reset(); 
  
    //TO SEE WHAT BLE SERVICES AND CHARACTERISTICS ARE CURRENTLY SET:
    ble.sendCommandCheckOK(F( "AT+GATTLIST" ));
    
  }

  //TO ENABLE ANDROID TRANSMISSION MODE:
  else if(bluetoothMode==1){
    
    //TO PERFORM FACTORY RESET:
    if( !ble.factoryReset() ){ Serial.println(" COULD NOT FACTORY RESET "); while(1); }

    //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
    ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=v^Speed PROTOTYPE[3]" )); ble.reset(); 

    //TO SEE WHAT BLE SERVICES AND CHARACTERISTICS ARE CURRENTLY SET:
    ble.sendCommandCheckOK(F( "AT+GATTLIST" ));
  
  }
  
}






//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//  DISPLAYS AN INTEGER UP TO FIVE CHARACTERS, 24pt FONT INSIDE A BOUNDING BOX  //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void PrintAltitude(int number){ //UP TO FIVE DIGITS, OR UP TO FOUR DIGITS WITH A NEGATIVE SIGN:

  //TEMPORARILY REMOVE THE NEGATIVE SIGN BEFORE CONVERSION TO ASCII:
  int absNumber = abs(number);
  
  //PARSE EACH DIGIT AND CONVERT TO ASCII CHARACTERS:
  int digit[] = {0,0,0,0,0};
  digit[4] = ((absNumber/10000)); //________________________________________________________________________________________________ = 0
  digit[3] = ((absNumber/1000) - (digit[4]*10)); //_________________________________________________________________________ = 4 - 0 = 4
  digit[2] = ((absNumber/100) - (digit[4]*100) - (digit[3]*10)); //___________________________________________________ = 45 - 0 - 40 = 5
  digit[1] = ((absNumber/10) - (digit[4]*1000) - (digit[3]*100) - (digit[2]*10)); //___________________________ = 451 - 0 - 400 - 50 = 1
  digit[0] = ((absNumber) - (digit[4]*10000) - (digit[3]*1000) - (digit[2]*100) - (digit[1]*10)); //___ = 4513 - 0 - 4000 - 500 - 10 = 3
  //EXAMPLE: digit[]={3,1,5,4,0} == ALTITUDE OF 4513;

  //SHIFT THE VALUES OF EACH DIGIT TO MATCH WITH ASCII TABLE: (' ' == 0 , '0' == 48 , '2' == 50 , ...)
  for(int i=0; i<=4; i++){digit[i]+=48;}

  //RE-INSERT NEGATIVE SIGN IF NEEDED AND TRIM LEADING ZEROS:
  if(number<0){
    if(digit[1]=='0' && digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
      digit[1]='-'; 
      for(int i=2; i<=4; i++){digit[i]=' ';}
    }
    else if(digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
      digit[2]='-';
      for(int i=3; i<=4; i++){digit[i]=' ';}
    }
    else if(digit[3]=='0' && digit[4]=='0'){
      digit[3]='-';
      digit[4]=' ';
    }
    else{
      digit[4]='-';
    }
  }

  //FOR POSITIVE NUMBERS, SIMPLY REMOVE LEADING ZEROS
  else{
    if(digit[1]=='0' && digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
      for(int i=1; i<=4; i++){digit[i]=' ';}
    }
    else if(digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
      for(int i=2; i<=4; i++){digit[i]=' ';}
    }
    else if(digit[3]=='0' && digit[4]=='0'){
      for(int i=3; i<=4; i++){digit[i]=' ';}
    }
    else if(digit[4]=='0'){
      digit[4]=' ';
    }    
  }
  
  //DEBUG:
  /*while(!Serial){};*/
  /*for(int i=4; i>=0; i--){delay(10); Serial.print(digit[i]);} Serial.println();*/  
  
  //BOUNDING BOX:
  paint.SetRotate(ROTATE_180);
  paint.SetWidth(128);
  paint.SetHeight(28);
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(1, 1, 88, 27, COLORED);
  for(int i=4; i>=0; i--){int j = 4-i; paint.DrawCharAt(2+(17*j), 4, digit[i], &Font24, COLORED);}
  epd.SetFrameMemory(paint.GetImage(), 0, /*184*/92, paint.GetWidth(), paint.GetHeight());
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//          DISPLAYS THE VELOCITY IN FEET PER SECOND AND NEEDLE WIDGET          //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void displayVario(int number){
  
  paint.SetRotate(ROTATE_180);
  paint.SetWidth(128);
  paint.SetHeight(158);
  paint.Clear(UNCOLORED);
  /*paint.DrawStringAt(1, 1, time_string, &Font16, COLORED);*/
  
  //NEEDLE POINTER:
  int numSwitch = number+10;
  Serial.println(numSwitch);
  switch(numSwitch){
    case 20: paint.DrawLine(42  , 151 , 114 , 91, COLORED); break;//10
    case 19: paint.DrawLine(37 , 145 , 111 , 91, COLORED); break;//9
    case 18: paint.DrawLine(33 , 138 , 109 , 91, COLORED); break;//8
    case 17: paint.DrawLine(29 , 132 , 105 , 91, COLORED); break;//7
    case 16: paint.DrawLine(26 , 125 , 104 , 90, COLORED); break;//6
    case 15: paint.DrawLine(23 , 117 , 104 , 88, COLORED); break;//5
    case 14: paint.DrawLine(20 , 110 , 104 , 86, COLORED); break;//4
    case 13: paint.DrawLine(18 , 102 , 104,  84, COLORED); break;//3
    case 12: paint.DrawLine(17 , 95 , 104 , 82, COLORED); break;//2
    case 11: paint.DrawLine(16 , 87 , 104 , 81, COLORED); break;//1
    default: paint.DrawLine(16  , 79  , 104 , 79, COLORED); break;//0
    case 10: paint.DrawLine(16  , 71  , 104 , 77, COLORED); break;//-1
    case 9: paint.DrawLine(17  , 63  , 104 , 76, COLORED); break;//-2
    case 8: paint.DrawLine(18  , 56  , 104 , 74, COLORED); break;//-3
    case 7: paint.DrawLine(20  , 48  , 104 , 72, COLORED); break;//-4
    case 6: paint.DrawLine(23  , 41  , 104 , 70, COLORED); break;//-5
    case 5: paint.DrawLine(26  , 33  , 104 , 68, COLORED); break;//-6
    case 4: paint.DrawLine(29  , 26  , 105 , 67, COLORED); break;//-7
    case 3: paint.DrawLine(33  , 20  , 109 , 67, COLORED); break;//-8
    case 2: paint.DrawLine(37  , 13  , 111 , 67, COLORED); break;//-9
    case 1: paint.DrawLine(42 , 7 , 114 , 67, COLORED); break;//-10
  }
  paint.DrawLine(0, 79, 128, 79, COLORED);

  epd.SetFrameMemory(paint.GetImage(), 0, 120, paint.GetWidth(), paint.GetHeight());
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//              DISPLAYS THE MAIN FLIGHT SCREEN DURING MAIN ACTIVITY            //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void EPAPER_MAIN(float altiFt, float veelawcityFtPerSec){
  //CLOCK:
  time_now_s = (millis() - time_start_ms) / 1000;
  char time_string[] = {'0', '0', ':', '0', '0', ':', '0', '0', '\0'};
  // 12:34:56 == 45296_s
  time_string[0] = time_now_s / 60 / 60 / 10 + '0'; //ten hr  ---> 45296 / 60 / 60 / 10 == 1
  time_string[1] = time_now_s / 60 / 60 / 10 + '0'; //one hr  ---> 45296 / 60 / 60 % 10 == 2
  time_string[3] = time_now_s / 60 / 10 + '0';      //ten min ---> 45296 / 60 % 60 / 10 == 3
  time_string[4] = time_now_s / 60 % 10 + '0';      //one min ---> 45296 / 60 % 10 == 4
  time_string[6] = time_now_s % 60 / 10 + '0';      //ten sec ---> 45296 % 60 / 10 == 5
  time_string[7] = time_now_s % 60 % 10 + '0';      //one sec ---> 45296 % 60 % 10 == 6

  //TOP BAR:
  paint.SetRotate(ROTATE_180);
  paint.SetWidth(128);
  paint.SetHeight(18);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(1, 1, time_string, &Font16, COLORED);
  paint.DrawHorizontalLine(0, 17, 128, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 278, paint.GetWidth(), paint.GetHeight());

  //PRINT VELOCITY:
  char velocity_string[] = {'-', '0', '0'};
  // -9_ft/s == {' ', '-', '9'};
  if(veelawcityFtPerSec<=-10){      velocity_string[0] = '-';    velocity_string[1] = abs(veelawcityFtPerSec/10)+'0';    velocity_string[2] = abs((int)veelawcityFtPerSec%10)+'0'; }
  else if(veelawcityFtPerSec<0){    velocity_string[0] = ' ';    velocity_string[1] = '-';                               velocity_string[2] = abs((int)veelawcityFtPerSec)+'0';    }
  else if(veelawcityFtPerSec<10){   velocity_string[0] = ' ';    velocity_string[1] = ' ';                               velocity_string[2] = (int)veelawcityFtPerSec+'0';         }
  else{                             velocity_string[0] = ' ';    velocity_string[1] = (veelawcityFtPerSec/10)+'0';       velocity_string[2] = ((int)veelawcityFtPerSec%10)+'0';    }
  paint.SetRotate(ROTATE_180);
  paint.SetWidth(128);
  paint.SetHeight(28);
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(1, 1, 51, 27, COLORED);
  paint.DrawStringAt(1, 4, velocity_string, &Font24, COLORED);
  /*paint.DrawHorizontalLine(0, 17, 128, COLORED);*/
  epd.SetFrameMemory(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());
  
  
  /*
   *  
   *   |<------112px------>|
   *   |    |<----86px---->|
   *  _|____|______________|________
   * |_______TOP_BAR_______|______|_----- 18px
   * |     /  \            |      ^
   * |   /       \         |      |
   * |  / VARIO     \      |      |
   * | |              _\___|      |
   * | |_____________|     |      |
   * | |        __,--|_____|     158px
   * | | __,---'           |      |
   * |  \                  |      |
   * |   \                 |      |
   * |_____\_______________|______v_
   * |       ALTITUDE      |      ^_____28px
   * |_____________________|______v_
   * |                     |      ^
   * |       CHART         |      |
   * |                     |     92px
   * |                     |      |
   * |                     |      |
   * |_____________________|______v_
   */

  /*//DIVIDER LINES:
  paint.SetRotate(ROTATE_180);
  paint.SetWidth(128);
  paint.SetHeight(2);
  paint.Clear(UNCOLORED);
  paint.DrawHorizontalLine(0, 1, 128, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 92, paint.GetWidth(), paint.GetHeight());*/

  //DISPLAY ALTITUDE:
  PrintAltitude((int)altiFt);

  //SIMPLER VELOCITY NEEDLE:
  SimplerVelocityNeedle(veelawcityFtPerSec);

  //DISPLAY VARIO:
  /*displayVario((int)veelawcityFtPerSec);*/
  
  epd.DisplayFrame();
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                          SIMPLER VELOCITY NEEDLE:                            //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void SimplerVelocityNeedle(float number){
  paint.SetRotate(ROTATE_180);
  paint.SetWidth(64);
  paint.SetHeight(120);
  paint.Clear(UNCOLORED);
  /*paint.DrawStringAt(1, 1, time_string, &Font16, COLORED);*/
  paint.DrawHorizontalLine(0,60,64,COLORED);
  paint.DrawLine(0, 59-5*number, 64, 59, COLORED);
  paint.DrawLine(0, 60-5*number, 64, 60, COLORED);
  paint.DrawLine(0, 61-5*number, 64, 61, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 120, paint.GetWidth(), paint.GetHeight());
}

