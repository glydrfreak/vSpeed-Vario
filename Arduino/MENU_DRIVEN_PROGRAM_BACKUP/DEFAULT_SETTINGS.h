#ifndef DEFAULT_SETTINGS_H
#define DEFAULT_SETTINGS_H

class Default{

  public:
  
    bool ENABLE_BEEP                 = 1;    // [1]YES, [0]NO
    int VOLUME                     = 100;    // DEFAULT 100%
    int VOLUME_MAX                 = 100;
    int VOLUME_MIN                   = 0;
    int VOLUME_INCREMENT            = 25;
    float CLIMB_BEEP_TRIGGER       = 1.0;    // (Default 1.0 ft)
    float SINK_ALARM_TRIGGER      = -1.0;    // (Default -1.0 ft/s)
    float CLIMB_PITCH_MAX        = 500.0;    // (Default 500.0 Hz)
    float CLIMB_PITCH_MIN        = 300.0;    // (Default 300.0 Hz)
    float SINK_PITCH_MAX         = 250.0;    // (Default 250.0 Hz)
    float SINK_PITCH_MIN         = 150.0;    // (Default 150.0 Hz)
    int BEEP_TYPE                    = 1;    // DEFAULT [1]BASED_ON_VELOCITY, [2]BUFFERED_INCREMENTS

    bool ENABLE_OLED                 = 1;    // [1]YES, [0]NO
    float CHART_SPEED               = 30;    // Seconds for completion; Live speed if zero;

    bool MEASURE_BATTERY             = 1;    // [1]YES, [0]NO

    bool ENABLE_BLUETOOTH            = 1;    // [1]YES, [0]NO
    int BLUETOOTH_MODE               = 2;    // [1]VSPEED_ANDROID, [2]FLYSKYHY_IOS
  
};
    
#endif