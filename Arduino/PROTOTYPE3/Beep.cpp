/*
	BEEP.cpp
	Variometer Beep Library written by Braedin and Paul Butler
	Function Definitions
*/

#include "BEEP.h"


//void BEEP::velocityBasedBeeps(float currentVelocity) 
//{

//}

void BEEP::basedOnAltitude(float currentAltitude, unsigned long currentTime){

  float velo = (1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory);
  
  //if(dbg){Serial.println();}
  
	/* (DECISION 1) Has there been a positive altitude change great enough to trigger a beep? */
	if(currentAltitude - altitudeTriggerMemory >= verticalTrigger)
	{
	  if(dbg){
      //Serial.print(" [D1Y] *BEEP* ");
      Serial.print(" *BEEP* ");
	    Serial.print(" velo:");Serial.print(velo);
      Serial.print(" ");
	  }
  
		/* (DECISION 2) Is the duration of the beep going to be too long? */
		if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong)
		{
      if(dbg) {
        //Serial.print(" [D2Y] ");
        Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); 
        Serial.print(" ");     
      }
			beepDuration = climbDurationLong; // Limit the beep duration
	  } 
		else{
			beepDuration = ((currentTime - timeTriggerMemory) / 2.0); // Don't limit the beep duration

      if(dbg) {
        //Serial.print(" [D2N] ");
        Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); 
        Serial.print(" ");
      }
		}
		
		// Determine pitch by mapping the values based on beepDuration
	  //beepPitch = (beepDuration - climbDurationLong) * (pitchMax - pitchMin)/(climbDurationShort - climbDurationLong) + pitchMin;  
    beepPitch = (((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin;
    
    if(dbg){
      Serial.print(" d:"); Serial.print(beepDuration); 
      Serial.print(" p:"); Serial.print(beepPitch); 
      Serial.println(" ");
    }
		tone(buzzerPin, beepPitch, beepDuration+(0.25*beepDuration));   // Activate the beep
		altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
		timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
	}
	else{
    //if(dbg) {Serial.print(" [D1N] ");}
		
		/* (DECISION 3) Has there been a negative altitude change great enough to trigger the sinkAlarm?  */
		if(altitudeTriggerMemory - currentAltitude >= verticalTrigger)
		{
      if(dbg) {
        //Serial.print(" [D3Y] "); 
        //Serial.print(" velo:");Serial.println(velo);
      }
      //int velo = (1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory);
			/* (DECISION 4) is the altitude dropping fast enough to trigger the sinkAlarm?  */
			if(velo <= sinkAlarm)	{
        sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);

	      if(dbg) {
          //Serial.print(" [D4Y] *SINK*");
          Serial.print(" *SINK_1* "); 
          Serial.print(" velo:");Serial.print(velo);
          Serial.print(" d:"); Serial.print(sinkAlarmDuration);
          Serial.print(" p:"); Serial.println(sinkAlarmPitch); 
        }

				//tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
        altitudeTriggerMemory = currentAltitude;            // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;                    // Use currentTime as the next reference point
			}
			else{
        if(dbg) {
          Serial.print(" [D4N] ");
          Serial.print(" velo:");Serial.println(velo);
        }
        
        altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
      }
    }           
		else{
      //if(dbg) {Serial.print(" [D3N] ");}
		}
	}
  if(velo <= sinkAlarm){
    if(dbg){
      Serial.print(" *SINK_2* "); 
      Serial.print(" velo:");Serial.print(velo);
      Serial.print(" d:"); Serial.print(sinkAlarmDuration);
      Serial.print(" p:"); Serial.println(sinkAlarmPitch);
    }
    sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);
    tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
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


