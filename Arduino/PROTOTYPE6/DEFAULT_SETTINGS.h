#ifndef DEFAULT_SETTINGS_H
#define DEFAULT_SETTINGS_H

class Default{
  public:
  
    bool ENABLE_BEEP                 = 1;    // [1]YES, [0]NO
    int START_UP_VOLUME             = 10;    // (0 to 31)
    int BEEP_TYPE                    = 1;    // DEFAULT [1]BASED_ON_VELOCITY, [2]BUFFERED_INCREMENTS
    
    float CLIMB_BEEP_TRIGGER       = 1.0;    // (Default 1.0 ft)
    float SINK_BEEP_TRIGGER       = -1.0;    // (Default -1.0 ft/s)

    float CLIMB_PITCH_MAX        = 500.0;    // (Default 500.0 Hz)
    float CLIMB_PITCH_MIN        = 350.0;    // (Default 300.0 Hz)
    
    float SINK_PITCH_MAX         = 250.0;    // (Default 250.0 Hz)
    float SINK_PITCH_MIN         = 150.0;    // (Default 150.0 Hz)
    
    bool MEASURE_BATTERY             = 1;    // [1]YES, [0]NO

    bool ENABLE_BLUETOOTH            = 1;    // [1]YES, [0]NO
    int START_UP_BLUETOOTH_MODE      = 1;    // [0]DISABLE, [1]VSPEED_ANDROID, [2]FLYSKYHY_IOS

};
    
#endif

