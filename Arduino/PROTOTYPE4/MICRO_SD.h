#ifndef MICRO_SD_H
#define MICRO_SD_H

#include <Arduino.h>

//#include <SPI.h>
#include <SD.h>



class MEMORY{
  
  private:
    
    File myFile;
    
//    struct USER{
//      String BRAEDIN = "BRAEDIN";
//      String PAUL = "PAUL";
//    };
    
    String USER = "BRAEDIN";
    String USER_SETTINGS_FILE = USER+".TXT";

    

  public:
    void storeVariable(String variableToStore, float valueToStore);
    float findVariable(String variableToFind);
    void switchUser(String userName){USER = userName;}
    void newUser(String userName);
  
    //VARIABLES TO LOOK FOR:
    String search_USER = "String USER = ";
    String search_ENABLE_BLUETOOTH = "bool ENABLE_BLUETOOTH = ";
    String search_BLUETOOTH_MODE = "int BLUETOOTH_MODE = ";
    String search_ENABLE_OLED = "bool ENABLE_OLED = ";
    String search_CHART_SPEED = "float CHART_SPEED = ";
    String search_ENABLE_BEEP = "bool ENABLE_BEEP = ";
    String search_BEEP_TYPE = "int BEEP_TYPE = ";
    String search_CLIMB_BEEP_TRIGGER = "float CLIMB_BEEP_TRIGGER = ";
    String search_SINK_ALARM_TRIGGER = "float SINK_ALARM_TRIGGER = ";
    String search_CLIMB_PITCH_MAX = "float CLIMB_PITCH_MAX = ";
    String search_CLIMB_PITCH_MIN = "float CLIMB_PITCH_MIN = ";
    String search_SINK_PITCH_MAX = "float SINK_PITCH_MAX = ";
    String search_SINK_PITCH_MIN = "float SINK_PITCH_MIN = ";
    String search_MEASURE_BATTERY = "bool MEASURE_BATTERY = ";
    String search_VOLUME = "int VOLUME = ";
    
    
};

#endif


//====NEW FILE SHOULD CONTAIN:====================================================
//    
//    //USER:
//    String USER = "userName";
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool ENABLE_BLUETOOTH = 1;
//    
//    //DEFAULT [1]VSPEED_ANDROID, [2]FLYSKYHY_IOS:
//    int BLUETOOTH_MODE = 1;
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool ENABLE_OLED = 1;
//    
//    //DEFAULT 30 seconds OF VELOCITY DISPLAYED ON CHART:
//    float CHART_SPEED = 30;
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool ENABLE_BEEP = 1;
//
//    //DEFAULT 100%:
//    int VOLUME = 100;
//    
//    //DEFAULT [1]BASED_ON_VELOCITY, [2]BUFFERED_INCREMENTS:
//    int BEEP_TYPE = 1;
//    
//    //DEFAULT 1.0 ft:
//    float CLIMB_BEEP_TRIGGER = 1.0;
//    
//    //DEFAULT -1.0 ft/s:
//    float SINK_ALARM_TRIGGER = -1.0;
//    
//    //DEFAULT 500.0 Hz:
//    float CLIMB_PITCH_MAX = 500.0;
//    
//    //DEFAULT 300.0 Hz:
//    float CLIMB_PITCH_MIN = 300.0;
//    
//    //DEFAULT 250.0 Hz:
//    float SINK_PITCH_MAX = 250.0;
//    
//    //DEFAULT 150.0 Hz:
//    float SINK_PITCH_MIN = 150.0;
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool MEASURE_BATTERY = 1;
//
//================================================================================

