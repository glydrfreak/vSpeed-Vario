/*
	BEEP.h
	Variometer Beep Library written by Braedin and Paul Butler
	
*/

#ifndef BEEP_h
#define BEEP_h

#include <Arduino.h>


class BEEP{


private:	
  #define DURATION 0                    //NEW
  #define PITCH 1                       //NEW
  #define beepBuffSize 5                //NEW
  float percentageOfCycleOn = 0.50;     //NEW
  bool allowInterrupt = true;           //NEW
  bool incrementDuration = true;        //NEW
  float beeps[2][beepBuffSize] = {};    //NEW
  int beepsWaitingToBeep = 0;           //NEW
  unsigned long beepMillis = 0;         //NEW
  bool needBeeps = false;               //NEW

	float altitudeTriggerMemory;
	unsigned long timeTriggerMemory;
	float beepDuration;
	int beepPitch;
  bool dbg = true;               // set true when debugging is needed
	int verticalTrigger = 1.0;		  // default feet
	int sinkAlarm = -1.0;		        // default feet per second
	int sinkAlarmDuration = 500;	// default milliseconds
	int sinkAlarmPitch = 250;	    // default Hz
	int sap = sinkAlarmPitch;	
  float climbDurationShort = 50.0;	// default milliseconds
	float climbDurationLong = 300.0;	// default milliseconds 
  float prevDur = climbDurationLong;
  
  

public:
  
  float pitchMax = 500.0;           // default Hz
  float pitchMin = 300.0;           // default Hz
  float sinkPitchMax = 250;
  float sinkPitchMin = 150;
  int beepWait = 0;
  int buzzerPin = A5; // default pin connected to the buzzer
  
  void basedOnVelocity(float currentAltitude, float velo, unsigned long currentTime);
  void basedOnAltitude(float currentAltitude, float velo, unsigned long currentTime);
  void durationIncrements(float currentAltitude, float velo, unsigned long currentTime);
  void bufferedDurationIncrements(float currentAltitude, float velo, unsigned long currentTime);
  
  
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




