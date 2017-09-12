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
	  if(dbg){
	    Serial.print(" [D1Y] *BEEP* ");
	    Serial.print(" a:");Serial.print(currentAltitude - altitudeTriggerMemory);
      Serial.print(" ");
	  }
  
		/* (DECISION 2) Is the duration of the beep going to be too long? */
		if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong)
		{
      if(dbg) {
        Serial.print(" [D2Y] ");
        Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); 
        Serial.print(" ");     
      }
			beepDuration = climbDurationLong; // Limit the beep duration
	  } 
		else{
			beepDuration = ((currentTime - timeTriggerMemory) / 2.0); // Don't limit the beep duration

      if(dbg) {
        Serial.print(" [D2N] ");
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
      Serial.print(" ");
    }
		tone(buzzerPin, beepPitch, beepDuration+(0.25*beepDuration));   // Activate the beep
		altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
		timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
	}
	else{
    if(dbg) {Serial.print(" [D1N] ");}
		
		/* (DECISION 3) Has there been a negative altitude change great enough to trigger the sinkAlarm?  */
		if(altitudeTriggerMemory - currentAltitude >= verticalTrigger)
		{
      if(dbg) {
        Serial.print(" [D3Y] a:"); 
        Serial.print(altitudeTriggerMemory - currentAltitude);
      }
      int velo = (1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory);
			/* (DECISION 4) is the altitude dropping fast enough to trigger the sinkAlarm?  */
			if((1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory) <= sinkAlarm)
			{
        

        int separate = 5;
        if(velo <= sinkAlarm-6){sinkAlarmPitch = sap - separate*6;}
        else if(velo <= sinkAlarm-5){sinkAlarmPitch = sap - separate*5;}
        else if(velo <= sinkAlarm-4){sinkAlarmPitch = sap - separate*4;}
        else if(velo <= sinkAlarm-3){sinkAlarmPitch = sap - separate*3;}
        else if(velo <= sinkAlarm-2){sinkAlarmPitch = sap - separate*2;}
        else if(velo <= sinkAlarm-1){sinkAlarmPitch = sap - separate;}
        else{sinkAlarmPitch = sap;}


	if(dbg) {
          Serial.print(" [D4Y] *SINK*"); 
          Serial.print(" d:"); Serial.print(sinkAlarmDuration);
          Serial.print(" p:"); Serial.print(sinkAlarmPitch); 
        }



				tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
        altitudeTriggerMemory = currentAltitude;            // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;                    // Use currentTime as the next reference point
			}
			else{
        if(dbg) {
          Serial.print(" [D4N] ");
        }
        altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
      }
    }           
		else{
      if(dbg) {
        Serial.print(" [D3N] ");
      }
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

