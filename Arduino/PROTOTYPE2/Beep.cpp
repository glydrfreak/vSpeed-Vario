/*
	Beep.cpp
	Variometer Beep Library written by Braedin Butler
	Function Definitions
*/

#include "Beep.h"


//void BEEP::velocityBasedBeeps(float currentVelocity) 
//{

//}

void BEEP::basedOnAltitude(float currentAltitude, unsigned long currentTime){
	
	/* (DECISION 1) Has there been a positive altitude change great enough to trigger a beep? */
	if(currentAltitude - altitudeTriggerMemory >= verticalTrigger)
	{
	  //if(dbg){Serial.print(" D1Y_triggeredWith:_");Serial.print(currentAltitude - altitudeTriggerMemory);Serial.print("_memory:_");Serial.print(altitudeTriggerMemory);Serial.print("_current:_");Serial.print(currentAltitude);}
    //if(dbg){Serial.print(" D1:Y ");}
    
		/* (DECISION 2) Is the duration of the beep going to be too long? */
		if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong)
		{
      //if(dbg) {Serial.print(" D2:Y ");}
			beepDuration = climbDurationLong; // Limit the beep duration
	  } 
		else{
      //if(dbg) {Serial.print(" D2:N ");}
			beepDuration = ((currentTime - timeTriggerMemory) / 2.0); // Don't limit the beep duration
      //if(dbg) {Serial.print("_currentTime:"); Serial.print(currentTime); Serial.print("-timeTriggerMemory:"); Serial.print(timeTriggerMemory); Serial.print("=beepDuration:"); Serial.print(beepDuration);}
		}
	  beepPitch = (beepDuration - climbDurationLong) * (pitchMax - pitchMin)/(climbDurationShort - climbDurationLong) + pitchMin;  // Determine pitch by mapping the values based on beepDuration
    //if(dbg){Serial.print("*BEEP*"); Serial.print(" beepPitch(Hz): "); Serial.print(beepPitch); Serial.print(" beepDuration(s): "); Serial.print(beepDuration); Serial.print(" ");}
		tone(buzzerPin, beepPitch, beepDuration);   // Activate the beep
		altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
		timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
	}
	else{
    //if(dbg) {Serial.print(" D1:N ");}
		
		/* (DECISION 3) Has there been a negative altitude change great enough to trigger the sinkAlarm?  */
		if(altitudeTriggerMemory - currentAltitude >= verticalTrigger)
		{
      //if(dbg) {Serial.print("_D3Y_triggeredWith:_"); Serial.print(altitudeTriggerMemory - currentAltitude);}
			//if(dbg){Serial.print(" D3:Y ");}
      
			/* (DECISION 4) is the altitude dropping fast enough to trigger the sinkAlarm?  */
			if((1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory) <= sinkAlarm)
			{
        //if(dbg) {Serial.print("_D4Y_*sinkAlarm*"); Serial.print(" sinkAlarmPitch(Hz): "); Serial.print(sinkAlarmPitch); Serial.print(" sinkAlarmDuration(s): "); Serial.print(sinkAlarmDuration);}
				//if(dbg){Serial.print(" D4:Y ");}
				tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
        altitudeTriggerMemory = currentAltitude;            // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;                    // Use currentTime as the next reference point
			}
			else{
        //if(dbg) {Serial.print("_D4N_shouldntInitiateAnything ");}
        //if(dbg){Serial.print(" D4:N ");}
        altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
      }
    }           
		else{
      //if(dbg) {Serial.print(" D3N_shouldntInitiateAnything ");}
      //if(dbg){Serial.print(" D3:N ");}
		}
	}
}


//====SETTINGS===========================================================/

// Set buzzerPin
void BEEP::begin(int BP){
  buzzerPin = BP;
} 
/*
// Set buzzerPin, climbBeepTrigger, sinkAlarmTrigger
void BEEP::begin(int BP, float VT, float SA){
  buzzerPin = BP; 
  verticalTrigger = VT; 
  sinkAlarm = SA;
}  
      
// Set buzzerPin, climbBeepTrigger, sinkAlarmTrigger, 
// climbPitchMax, climbPitchMin, sinkAlarmPitch
void BEEP::begin(int BP, float VT, float SA, float HUBP, float LUBP, float SAP){
  buzzerPin = BP; 
  verticalTrigger = VT; 
  sinkAlarm = SA;
  pitchMax = HUBP; 
  pitchMin = LUBP; 
  sinkAlarmPitch = SAP;
} 


//====INDIVIDUAL_SETTINGS===================================================/

void BEEP::setVerticalTrigger(float VT){
  verticalTrigger = VT;
}
    
void BEEP::setSinkAlarmTrigger(float SA){ 
  sinkAlarm = SA;
}
  
void BEEP::setSinkAlarmDuration(float SAD){
  sinkAlarmDuration = SAD;
}

void BEEP::setSinkAlarmPitch(float SAP){
  sinkAlarmPitch = SAP;
}

void BEEP::setClimbPitchMax(float HUBP){
  pitchMax = HUBP;
}

void BEEP::setClimbPitchMin(float LUBP){
  pitchMin = LUBP;
}

void BEEP::setClimbDurationMin(float HUBD){
  climbDurationShort = HUBD;
}

void BEEP::setClimbDurationMax(float LUBD){
  climbDurationLong = LUBD;
} 

void BEEP::debug(bool d_bug){
  dbg = d_bug;
}
*/
