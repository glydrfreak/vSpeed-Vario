/*
	Beep.h
	Variometer Beep Library written by Braedin Butler
	
*/

#ifndef Beep_h
#define Beep_h

#include <Arduino.h>


class BEEP{


private:	
	float altitudeTriggerMemory;
	unsigned long timeTriggerMemory;
	int beepDuration;
	int beepPitch;
  bool dbg = true;               // set true when debugging is needed
	int verticalTrigger = 1.0;		  // default feet
	
	int sinkAlarm = -10.0;		        // default feet per second
	int sinkAlarmDuration = 500;	// default milliseconds
	int sinkAlarmPitch = 200;	    // default Hz
	float climbDurationShort = 50.0;	// default milliseconds
	float climbDurationLong = 500.0;	// default milliseconds 

public:
  float pitchMax = 900.0;           // default Hz
  float pitchMin = 700.0;           // default Hz
  // TODO -- void basedOnVelocity(float currentVelocity);
  void basedOnAltitude(float currentAltitude, unsigned long currentTime);
  int buzzerPin = A2; // default pin connected to the buzzer
  
  // set buzzerPin
  void begin(int BP); 

  /*
  // set buzzerPin, climbBeepTrigger, sinkAlarmTrigger
  void begin(int BP, float VT, float SA); 
      
  // set buzzerPin, climbBeepTrigger, sinkAlarmTrigger, 
  // climbPitchMax, climbPitchMin, sinkAlarmPitch
  void begin(int BP, float VT, float SA, float HUBP, float LUBP, float SAP); 
  
  void setVerticalTrigger(float VT);
  void setSinkAlarmTrigger(float SA);
  void setSinkAlarmDuration(float SAD);
  void setSinkAlarmPitch(float SAP);
  void setClimbPitchMax(float HUBP);
  void setClimbPitchMin(float LUBP);
  void setClimbDurationMin(float HUBD);
  void setClimbDurationMax(float LUBD);
  void debug(bool d_bug);
*/
  
};

#endif


