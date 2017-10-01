/*
	BEEP.h
	Variometer Beep Library written by Braedin and Paul Butler
	
*/

#ifndef BEEP_h
#define BEEP_h

#include <Arduino.h>


class BEEP{


private:	
	float altitudeTriggerMemory;
	unsigned long timeTriggerMemory;
	int beepDuration;
	int beepPitch;
  bool dbg = false;               // set true when debugging is needed
	int verticalTrigger = 1.0;		  // default feet
	
	int sinkAlarm = -1.0;		        // default feet per second
	int sinkAlarmDuration = 500;	// default milliseconds
	int sinkAlarmPitch = 200;	    // default Hz
	int sap = sinkAlarmPitch;	
  float climbDurationShort = 50.0;	// default milliseconds
	float climbDurationLong = 500.0;	// default milliseconds 

public:
  float pitchMax = 900.0;           // default Hz
  float pitchMin = 700.0;           // default Hz
  float sinkPitchMax = 200;
  float sinkPitchMin = 150;
  // TODO -- void basedOnVelocity(float currentVelocity);
  void basedOnAltitude(float currentAltitude, float velo, unsigned long currentTime);
  int buzzerPin = A5; // default pin connected to the buzzer
  
  // set buzzerPin
  void begin(int BP); 
  
  void setClimbThreshold(float VT){verticalTrigger = VT;}
  void setSinkAlarmThreshold(float SA){sinkAlarm = SA;}
  void setClimbPitchMax(float HUBP){pitchMax = HUBP;}
  void setClimbPitchMin(float LUBP){pitchMin = LUBP;}
  void setSinkPitchMax(float SPX){sinkPitchMax = SPX;}
  void setSinkPitchMin(float SPN){sinkPitchMin = SPN;}
  //void setSinkAlarmPitch(float SAP){sinkAlarmPitch = SAP; sap = SAP;}
  
};

#endif




