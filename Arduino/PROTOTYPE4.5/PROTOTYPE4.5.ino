//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                       v^SPEED VARIO -- PROTOTYPE[4.5]                        //
//                        (Vertical Speed Indicator)                            //
/*           Writen by Braedin Butler, with many other contributers             */
//BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|

#include "DEFAULT_SETTINGS.h"
#include "MENU.h"
#include "BUTTON.h"
#include "OLED.h"
#include "BEEP.h"
#include "MICRO_SD.h"
#include "vAdafruit_BluefruitLE_SPI.h"
#include "MS5611.h"
#include "FILTER.h"

#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define POWER_PIN                     A0    // DEVICE POWERS OFF IF THIS PIN GOES LOW;
#define BUTTON_PIN                    A1    // CONNECT THE OTHER BUTTON LEAD TO 3.3V;
#define SD_PIN                        A2
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin
#define MS5611_CSB                    13    // Chip/Slave Select Pin
#define D1_OSR                         5    // (Default pressure OSR mode 5) 
#define D2_OSR                         2    // (Default temperature OSR mode 2) 
#define VELOS_AVGERAGED               15    // Number of most recent velocity values averaged; max is maxVeloData set in MS5611.h
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer 
#define VOLUME_MAX                   100
#define VOLUME_MIN                     0
#define CLIMB_THRESH_MAX            10.0
#define CLIMB_THRESH_MIN             1.0
#define SINK_THRESH_MAX             -1.0
#define SINK_THRESH_MIN            -10.0
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
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);
vAdafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void SWITCH_BLE_MODE(int bluetoothMode);    // BLE MODULE INITIALIZES ACCORDING TO SPECIFIED BLUETOOTH_MODE;
void receiveCommands();
void transmitVspeed();
void transmitFlySkyHy();
void drawPage();
float getBatteryLvl();
void scrollingAltitude(float scrolledAlti); // Custom OLED Widget
void liveChart(int v, float speed);         // Custom OLED Widget
void batteryIcon(float battLvl);            // Custom OLED Widget

bool displayOn = true;
float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
int AVERAGING_DURATION = 1000;    // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
long battMillis = -5000;
float batteryLvl = 0;
int batteryPercent = 100;



enum {//POSSIBLE ACTIVITY NAMES:
  MAIN_ACTIVITY, SETTINGS_ACTIVITY, BEEP_ACTIVITY, VOLUME_ACTIVITY, THRESHOLD_ACTIVITY,
  CLIMB_ACTIVITY, SINK_ACTIVITY, PITCH_ACTIVITY, CLIMB_MAX_ACTIVITY, CLIMB_MIN_ACTIVITY,
  SINK_MAX_ACTIVITY, SINK_MIN_ACTIVITY, BLUETOOTH_ACTIVITY, OLED_ACTIVITY, CHART_SPEED_ACTIVITY,
  USER_ACTIVITY, EDIT_NAMES_ACTIVITY, USER_1_ACTIVITY, USER_2_ACTIVITY, USER_3_ACTIVITY, POWER_OFF_ACTIVITY
};



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                                    SETUP                                     //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
void setup() {
  Serial.begin(BAUD_RATE);
  //while(!Serial);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);  //ONCE THIS PIN GOES LOW, THE DEVICE POWERS OFF;

  //CHECK IF SD IS INSERTED, ESPECIALLY TO UPDATE DEFAULT PREFERENCES:
  if(SD.begin(SD_PIN)){ 
    SETTING.BLUETOOTH_MODE = STORAGE.findVariable(STORAGE.search_BLUETOOTH_MODE);
    //TODO-- ...
  }
  
  ble.begin(VERBOSE_MODE);

  MS5611.begin(MS5611_CSB);

  BEEP.begin(BEEP_PIN);
  BEEP.setClimbThreshold(SETTING.CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SETTING.SINK_ALARM_TRIGGER);   //ft/s
  BEEP.setClimbPitchMax(SETTING.CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(SETTING.CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SETTING.SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SETTING.SINK_PITCH_MIN);             //Hz

  if(SETTING.ENABLE_OLED){ 
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.setFontType(0);
    oled.drawBitmap(oled.logo);
    oled.display();         // Display what's in the buffer (splashscreen)
  }

  SWITCH_BLE_MODE(SETTING.BLUETOOTH_MODE);  //INITIALIZES THE SPECIFIED BLUETOOTH MODE;  
  
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                                     LOOP                                     //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
void loop() {
  
  currentMillis = millis();
  samplesThisSec++; //increment each time the loop cycles
  // Update value of samplesPerSec once every second:
  if(currentMillis - previousMillis >= 1000){
    previousMillis=currentMillis;
    samplesPerSec = samplesThisSec;
    samplesThisSec=0; 
    //print debug info
    Serial.println(samplesPerSec);
  }



    /*#######################################*/
   ////////////////MAIN ACIVITY//////////////
  /*######################################*/
  while(M.CURRENT_PAGE==MAIN_ACTIVITY){

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
    //TODO--OBTAIN STARTUP VOLUME FROM MICRO SD;
    if(SETTING.ENABLE_BEEP && currentMillis > 4000){                                                                //BEEP
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

  //====OLED===================================================================/
    if(SETTING.ENABLE_OLED && currentMillis>2000){
      displayOn = true;
      oled.clear(PAGE);  //Clear the screen
    }
    //oled.line(random(0,64), random(0,48), random(0,64), random(0,48));
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
      M.CURRENT_PAGE=SETTINGS_ACTIVITY;
    }
    if(SETTING.ENABLE_OLED && currentMillis>2000){
      liveChart(velocityFtPerSec, SETTING.CHART_SPEED);
      if(SETTING.MEASURE_BATTERY){batteryIcon(batteryLvl);}
      scrollingAltitude(altitudeFt);
      if(velocityFtPerSec>=0){oled.setCursor(50,41);}
      else{oled.setCursor(44,41);}
      oled.print(velocityFtPerSec,0);
      oled.display();   //Draw the new screen
    }
    else if(displayOn){
      displayOn = false;
      oled.clear(PAGE);
      oled.display();
    }
  }


    /*#######################################*/
   /////////////////POWER OFF////////////////
  /*######################################*/ 
  if(M.CURRENT_PAGE==POWER_OFF_ACTIVITY){digitalWrite(POWER_PIN, LOW);}

  
    /*#######################################*/
   ////////////////SETTINGS MENU//////////////
  /*######################################*/  
  M.menuItem( "SETTINGS", ">BEEP", ">BTOOTH", ">DISPLAY", ">USER", "<-*", "OFF" );
  M.itemPurpose(M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER);
  M.ifSelectedGoTo(M.NONE, BEEP_ACTIVITY, BLUETOOTH_ACTIVITY, OLED_ACTIVITY, USER_ACTIVITY, MAIN_ACTIVITY, POWER_OFF_ACTIVITY);
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==SETTINGS_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   /////////////////BEEP MENU////////////////
  /*######################################*/                                                                                            
  M.menuItem( "BEEP", ">VOLUME*", ">THRESH", ">PITCH", " ", "<-", "->" );                                                                                                                                  
  M.itemPurpose(M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER);   
  M.ifSelectedGoTo(M.NONE, VOLUME_ACTIVITY, THRESHOLD_ACTIVITY, PITCH_ACTIVITY, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY);                                                   
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );                                                           
  while(M.CURRENT_PAGE==BEEP_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}
  


    /*#######################################*/
   ////////////////VOLUME MENU///////////////
  /*######################################*/  
  M.menuItem( "VOL=100", "+25*", "-25", "(ON)", " ", "<-", "->" );
  M.itemPurpose(M.NAMED_INT, M.INT_ADJUSTER, M.INT_ADJUSTER, M.BOOL_TOGGLER, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER);   //POSSIBLE PURPOSES WHEN CORRESPONDING ITEM IS SELECTED;
  M.ifSelectedGoTo(M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, BEEP_ACTIVITY, MAIN_ACTIVITY);                                                //POSSIBLE NEXT PAGES WHEN CORRESPONDING ITEM IS SELECTED;
  M.integerAdjust(VOLUME_MIN, VOLUME_MAX);                                                                      //PARAMETERS TO ADJUST THE NAMED_INT VALUE DISPLAYED;
  M.booleanToggle( "(ON)", "(MUTED)" );                                                                                         //BOOLEAN VARIABLE TO SWAP BETWEEN TRUE AND FALSE;
  M.initializeActivity( SETTING.VOLUME, M.NO_FLOAT, SETTING.ENABLE_BEEP );                                                      //INITIALIZE THE ACTIVITY;
  while(M.CURRENT_PAGE==VOLUME_ACTIVITY){M.launchActivity( SETTING.VOLUME, M.NO_FLOAT, SETTING.ENABLE_BEEP ); drawPage();}                  //START THE ACTIVITY; drawPage() IS CREATED BY USER;
  //STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);
  //STORAGE.storeVariable(STORAGE.search_ENABLE_BEEP, SETTING.ENABLE_BEEP);



    /*#######################################*/
   ///////////////THRESHOLD MENU/////////////
  /*######################################*/   
  M.menuItem( "THRESHOLD", ">CLIMB*", ">SINK", " ", " ", "<-", "->" );
  M.itemPurpose( M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CLIMB_ACTIVITY, SINK_ACTIVITY, M.NONE, M.NONE, BEEP_ACTIVITY, MAIN_ACTIVITY );
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==THRESHOLD_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}



    /*#######################################*/
   ////////////////CLIMB MENU////////////////
  /*######################################*/    
  M.menuItem( "CLIMB=1", "+1*", "-1", " ", " ", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, THRESHOLD_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust( CLIMB_THRESH_MIN, CLIMB_THRESH_MAX );                                  
  M.initializeActivity( M.NO_INT, SETTING.CLIMB_BEEP_TRIGGER, M.NO_BOOL );
  while(M.CURRENT_PAGE==CLIMB_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.CLIMB_BEEP_TRIGGER, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   /////////////////SINK MENU////////////////
  /*######################################*/   
  M.menuItem( "SINK=-10", "+1*", "-1", " ", " ", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, THRESHOLD_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust( SINK_THRESH_MIN, SINK_THRESH_MAX);                                  
  M.initializeActivity( M.NO_INT, SETTING.SINK_ALARM_TRIGGER, M.NO_BOOL );
  while(M.CURRENT_PAGE==SINK_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.SINK_ALARM_TRIGGER, M.NO_BOOL ); drawPage();}

    /*#######################################*/
   ////////////////PITCH MENU////////////////
  /*######################################*/     
  M.menuItem( "PITCH", ">CLMBmax*", ">CLMBmin", ">SINKmax", ">SINKmin", "<-", "->" );
  M.itemPurpose( M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CLIMB_MAX_ACTIVITY, CLIMB_MIN_ACTIVITY, SINK_MAX_ACTIVITY, SINK_MIN_ACTIVITY, BEEP_ACTIVITY, MAIN_ACTIVITY );
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==PITCH_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ///////////////CLIMB_MAX MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.CLIMB_PITCH_MAX, M.NO_BOOL );
  while(M.CURRENT_PAGE==CLIMB_MAX_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.CLIMB_PITCH_MAX, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ///////////////CLIMB_MIN MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.CLIMB_PITCH_MIN, M.NO_BOOL ); 
  while(M.CURRENT_PAGE==CLIMB_MIN_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.CLIMB_PITCH_MIN, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ////////////////SINK_MAX MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.SINK_PITCH_MAX, M.NO_BOOL );
  while(M.CURRENT_PAGE==SINK_MAX_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.SINK_PITCH_MAX, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   ////////////////SINK_MIN MENU/////////////
  /*######################################*/     
  M.menuItem( "Hz=", "+50*", "+10", "-10", "-50", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, PITCH_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust(10, 4000);                                   
  M.initializeActivity( M.NO_INT, SETTING.SINK_PITCH_MIN, M.NO_BOOL );
  while(M.CURRENT_PAGE==SINK_MIN_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.SINK_PITCH_MIN, M.NO_BOOL ); drawPage();} 
        

    /*#######################################*/
   ///////////////BLUETOOTH MENU/////////////
  /*######################################*/
  M.menuItem( "MODE:", " v^Speed*", "*Flyskyhy", " ", " ", "<-", "->" );  
  M.itemPurpose( M.NONE, M.ITEM_SWITCHER, M.ITEM_SWITCHER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  M.initializeActivity( SETTING.BLUETOOTH_MODE, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==BLUETOOTH_ACTIVITY){M.launchActivity( SETTING.BLUETOOTH_MODE, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


    /*#######################################*/
   /////////////////OLED MENU////////////////
  /*######################################*/     
  M.menuItem( "DISP=ON", ">CHART*", " SPEED", " ", " ", "<-", "->" );
  M.itemPurpose( M.BOOL_TOGGLER, M.ACTIVITY_CHANGER, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CHART_SPEED_ACTIVITY, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  M.booleanToggle( "DISP=ON", "DISP=OFF");
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, SETTING.ENABLE_OLED );  
  while(M.CURRENT_PAGE==OLED_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, SETTING.ENABLE_OLED ); drawPage();}


    /*#######################################*/
   //////////////CHART_SPEED MENU////////////
  /*######################################*/     
  M.menuItem( "CHART=30", "(seconds)", "+5*", "-5", " ", "<-", "->" );
  M.itemPurpose( M.NAMED_FLOAT, M.NONE, M.FLOAT_ADJUSTER, M.FLOAT_ADJUSTER, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  M.floatAdjust(0, 120);                                  
  M.initializeActivity( M.NO_INT, SETTING.CHART_SPEED, M.NO_BOOL /*,NO_STRING*/ );
  while(M.CURRENT_PAGE==CHART_SPEED_ACTIVITY){M.launchActivity( M.NO_INT, SETTING.CHART_SPEED, M.NO_BOOL ); drawPage();}


     //TODO-- DO I REALLY NEED TO ADD INTERFACE FOR USERS???
    /*#######################################*/
   //////////////////USER MENU///////////////
  /*######################################*/     
  M.menuItem( "USER", "*BRAEDIN*", " PAUL", " ", " ", "<-", "->" );
  M.itemPurpose( M.NONE, M.ITEM_SWITCHER, M.ITEM_SWITCHER, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  M.initializeActivity( SETTING.USER, M.NO_FLOAT, M.NO_BOOL /*, SETTING.USER_NAME_1*/ );
  while(M.CURRENT_PAGE==USER_ACTIVITY){M.launchActivity( SETTING.USER, M.NO_FLOAT, M.NO_BOOL ); drawPage();}

  

     //TODO-- (A LOT OF WORK IF I WANT TO IMPLEMENT THE FOLLOWING ACTIVITIES)
    /*#######################################*/
   ///////////////EDIT_NAMES MENU////////////
  /*######################################*/     
  M.menuItem( "THRESHOLD", ">CLIMB*", ">SINK", " ", " ", "<-", "->" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==EDIT_NAMES_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}
  

     //TODO--
    /*#######################################*/
   ////////////////USER_1 MENU///////////////
  /*######################################*/     
  M.menuItem( "USER_1", " ", " ", " ", " ", "<-", "->*" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); 
  while(M.CURRENT_PAGE==USER_1_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}

    
    
     //TODO--
    /*#######################################*/
   ////////////////USER_2 MENU///////////////
  /*######################################*/     
  M.menuItem( "USER_2", " ", " ", " ", " ", "<-", "->*" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, CLIMB_ACTIVITY, SINK_ACTIVITY, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==USER_2_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


     //TODO--
    /*#######################################*/
   ////////////////USER_3 MENU///////////////
  /*######################################*/     
  M.menuItem( "USER_3", " ", " ", " ", " ", "<-", "->*" );
  M.itemPurpose( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, M.ACTIVITY_CHANGER, M.ACTIVITY_CHANGER );
  M.ifSelectedGoTo( M.NONE, M.NONE, M.NONE, M.NONE, M.NONE, SETTINGS_ACTIVITY, MAIN_ACTIVITY );
  //M.integerAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;
  //M.floatAdjust(SETTING._MIN, SETTING._MAX);  //COMMENT THIS LINE IF UNUSED;                                   
  //M.booleanToggle( "(ON)", "(OFF)"); //COMMENT THIS LINE IF UNUSED;
  M.initializeActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL );
  while(M.CURRENT_PAGE==USER_3_ACTIVITY){M.launchActivity( M.NO_INT, M.NO_FLOAT, M.NO_BOOL ); drawPage();}


}

//=============================================================================================================
/*************************************************************************************************************/





//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                                  BATTERY ICON                                //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void batteryIcon(float battLvl){
  int bars = round((((3 - 0) / (4.2 - 3.5)) * (battLvl - 3.5)) + 0);
  oled.rect(57,0, 6,4);
  oled.rect(58,1, 1+bars,2);
  oled.pixel(63,1);
  oled.pixel(63,2);
}





//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                        TO MEASURE BATTERY LEVEL                             //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
float getBatteryLvl(){  
  pinMode(VBATPIN, INPUT);
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
      //oled.pixel(i, y[i]);  // Draw all the new pixels except the most recent
      oled.line(i,y[i], i+1,y[i+1]);
    }

    y[63] = p;
    if(y[63]>47){y[63]=47;}
    else if(y[63]<0){y[63]=0;}
  
    //oled.pixel(63, y[63]);  // Draw the most recent pixel
    oled.line(62,y[62], 63,y[63]);

  }

  //OTHERWISE, EVERYTHING STAYS THE SAME
  else{ for(int i = 0; i < 63; i++){oled.line(i,y[i], i+1,y[i+1]);} }
    
  //DRAW THE ZERO AXIS
  for(int i = 3; i < 60; i+=4){ oled.pixel(i, 24); }
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                           SCROLLING ALTITUDE WIDGET                          //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void scrollingAltitude(float scrolledAlti){
  int Position = (((scrolledAlti - (int)scrolledAlti)*50));  //Calculate scrolledAlti's position
  oled.setCursor(2,Position-24);oled.print(scrolledAlti,0);
  oled.setCursor(2,Position+24);oled.print(scrolledAlti-1,0);
  oled.rect(0,22,27,11);
  oled.rectFill(1, 23, 25, 9, 0, 0);
  oled.setCursor(2,24);
  oled.print(scrolledAlti,0);
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                           TO DRAW EACH MENU ITEM                             //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
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
      oled.clear(ALL);        // Clear the display's internal memory
      SETTING.ENABLE_OLED = 0;
      //Serial.println("  oled:OFF");
    }
    else if(command == "D"){
      //Serial.println("  oled:ON");
      SETTING.ENABLE_OLED = 1;    
      oled.clear(ALL);        // Clear the display's internal memory
      oled.drawBitmap(oled.logo);  // Draw v^SPEED logo
      oled.display();         // Display what's in the buffer (splashscreen)
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
