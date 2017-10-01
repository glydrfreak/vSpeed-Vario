/*
	BEEP.cpp
	Variometer Beep Library written by Braedin and Paul Butler
	Function Definitions
*/

#include "BEEP.h"


//void BEEP::velocityBasedBeeps(float currentVelocity) 
//{

//}

void BEEP::basedOnAltitude(float currentAltitude, float velo, unsigned long currentTime){
  if(currentTime-timeTriggerMemory<50){return;} //...hopefully you'll never be climbing or sinking more than 20 feet per second...
  //float velo = (1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory);
  
  //if(dbg){Serial.print(" WELCOME" );}
  
	/* (DECISION 1) Has there been a positive altitude change great enough to trigger a beep? */
	if(currentAltitude - altitudeTriggerMemory >= verticalTrigger)
	{
	  //if(dbg){
      //Serial.print(" [D1Y] *BEEP* ");
      //Serial.print(" *BEEP* ");
	    //Serial.print(" velo:");Serial.print(velo);
      //Serial.print(" ");
	  //}
  
		/* (DECISION 2) Is the duration of the beep going to be too long? */
		if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong)
		{
      //if(dbg) {
        //Serial.print(" [D2Y] ");
        //Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); 
        //Serial.print(" ");     
      //}
			beepDuration = climbDurationLong; // Limit the beep duration
	  } 
		else{
			beepDuration = ((currentTime - timeTriggerMemory) / 2.0); // Don't limit the beep duration
      if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}
      //if(dbg) {
        //Serial.print(" [D2N] ");
        //Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); 
        //Serial.print(" ");
      //}
		}
		
		// Determine pitch by mapping the values based on velocity
    //beepPitch = (((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin;
    beepPitch = (((pitchMax - pitchMin) / (verticalTrigger+10.0 - verticalTrigger)) * (velo - verticalTrigger)) + pitchMin;
    if(beepPitch<pitchMin){beepPitch=pitchMin;}
    if(beepPitch>pitchMax){beepPitch=pitchMax;}
    
    //if(dbg){
      //Serial.print(" d:"); Serial.print(beepDuration); 
      //Serial.print(" p:"); Serial.print(beepPitch); 
      //Serial.println(" ");
    //}
    //beepDuration = beepDuration+(0.25*beepDuration);  //make each beep a bit longer
		if(velo<1){beepWait = 2*climbDurationLong;}
		else{beepWait = 2*beepDuration;}
		tone(buzzerPin, beepPitch, 1.25*beepDuration);   // Activate the beep
		altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
		timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
	}
	else{
    //if(dbg) {Serial.print(" [D1N] ");}
		
		/* (DECISION 3) Has there been a negative altitude change great enough to trigger the sinkAlarm?  */
		if(altitudeTriggerMemory - currentAltitude >= verticalTrigger)
		{
      //if(dbg) {
        //Serial.print(" [D3Y] "); 
        //Serial.print(" velo:");Serial.println(velo);
      //}
      //int velo = (1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory);
			/* (DECISION 4) is the altitude dropping fast enough to trigger the sinkAlarm?  */
			if(velo <= sinkAlarm)	{
        sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);

	      //if(dbg) {
          //Serial.print(" [D4Y] *SINK*");
          //Serial.print(" *SINK_1* "); 
          //Serial.print(" velo:");Serial.print(velo);
          //Serial.print(" d:"); Serial.print(sinkAlarmDuration);
          //Serial.print(" p:"); Serial.println(sinkAlarmPitch); 
        //}

				//tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
        altitudeTriggerMemory = currentAltitude;            // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;                    // Use currentTime as the next reference point
			}
			else{
        //if(dbg) {
          //Serial.print(" [D4N] ");
          //Serial.print(" velo:");Serial.println(velo);
        //}
        
        altitudeTriggerMemory = currentAltitude;    // Use currentAltitude as the next reference point
        timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
      }
    }           
		else{
      //if(dbg) {Serial.print(" [D3N] ");}
		}
	}
  if(velo <= sinkAlarm){
    //if(dbg){
      //Serial.print(" *SINK_2* "); 
      //Serial.print(" velo:");Serial.print(velo);
      //Serial.print(" d:"); Serial.print(sinkAlarmDuration);
      //Serial.print(" p:"); Serial.println(sinkAlarmPitch);
    //}
    sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);
    tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
  }

}



void BEEP::Smooth(float currentAltitude, float velo, unsigned long currentTime){
  
  //Prevents audio crashing ...hopefully you'll never be climbing or sinking more than 20 feet per second...
  if(currentTime-timeTriggerMemory<50){return;} 
    
  /* (DECISION 1) Has there been a positive altitude change great enough to trigger a beep? */
  if(currentAltitude - altitudeTriggerMemory >= verticalTrigger)
  {

    //print debug info
    if(dbg){Serial.print(" [D1Y] *BEEP* "); Serial.print(" *BEEP* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" trig:");Serial.print(currentAltitude - altitudeTriggerMemory); Serial.print(" ");}
  
    /* (DECISION 2) Is the duration of the beep going to be too long? */
    if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong){
      
      //print debug info
      if(dbg) {Serial.print(" [D2Y] "); Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); Serial.print(" ");}
      
      // Limit the beep duration
      beepDuration = climbDurationLong; 
    } 
    else{ 
      
      // Don't limit the beep duration
      beepDuration = ((currentTime - timeTriggerMemory) / 2.0);

      //print debug info
      if(dbg) {Serial.print(" [D2N] "); Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); Serial.print(" "); Serial.print(" RAW:");Serial.print(beepDuration);}
      
      // 1.3 == 30% increase
      if(beepDuration > 1.3*prevDur){beepDuration = 1.3*prevDur;}
      
      // 0.7 == 30% decrease
      else if(beepDuration < 0.7*prevDur){beepDuration = 0.7*prevDur;}
      
      //no duration change
      else{beepDuration = prevDur;}

      //if not climbing anymore, reset previous beep duration to maximum
      if(velo <= 0){prevDur = climbDurationLong;}

      //keep beepDuration within bounds
      if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}
      
    }
    
    // Determine pitch by mapping the values based on beepDuration
    beepPitch = (((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin;

    // OR Determine pitch by mapping the values based on velo
    //beepPitch = (((pitchMax - pitchMin) / (verticalTrigger+10.0 - verticalTrigger)) * (velo - verticalTrigger)) + pitchMin;

    //keep pitch inside of bounds
    if(beepPitch<pitchMin){beepPitch=pitchMin;}
    if(beepPitch>pitchMax){beepPitch=pitchMax;}

    //print debug info
    if(dbg){Serial.print(" Pd:"); Serial.print(prevDur); Serial.print(" d:"); Serial.print(beepDuration); Serial.print(" p:"); Serial.print(beepPitch); Serial.println(" ");}

    //adjust the wait time for beginning a new cycle
    if(velo<1){beepWait = 2*climbDurationLong;}
    else{beepWait = 2*beepDuration;}
    
    // Activate the beep
    tone(buzzerPin, beepPitch, 1.25*beepDuration); 

    //if a glitch or jump in data happens, use a new altitude reference point for next time
    if(currentAltitude > verticalTrigger+5 + altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}
    
    //if climbing, increment altitudeTriggerMemory
    else if(currentAltitude > verticalTrigger + altitudeTriggerMemory){altitudeTriggerMemory += verticalTrigger;}

    //if sinking, use a new altitude reference point for next time
    else if(currentAltitude < altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}
    
    // Use currentTime as the next reference point
    timeTriggerMemory = currentTime;            
  }
  else{

    //print debug info
    //if(dbg) {Serial.print(" [D1N] ");}
    
    /* (DECISION 3) Has there been a negative altitude change great enough to trigger the sinkAlarm?  */
    if(altitudeTriggerMemory - currentAltitude >= verticalTrigger){

      //print debug info
      if(dbg) {Serial.print(" [D3Y] "); Serial.print(" trig:"); Serial.print(altitudeTriggerMemory - currentAltitude); Serial.print(" velo:");Serial.println(velo);}
      
      //if a glitch or jump in data happens, use a new altitude reference point for next time
      if(currentAltitude > verticalTrigger+5 + altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}  
        
      //if climbing, increment altitudeTriggerMemory
      else if(currentAltitude > verticalTrigger + altitudeTriggerMemory){altitudeTriggerMemory += verticalTrigger;}     
        
      //if sinking, use a new altitude reference point for next time
      else if(currentAltitude < altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}                 
        
      // Use currentTime as the next reference point
      timeTriggerMemory = currentTime; 
    }           
  }


  //if we're sinking faster than specified, just blare a sink alarm (replaces DECISION 4)
  if(velo <= sinkAlarm){
    
    //print debug info
    if(dbg){Serial.print(" *SINK_2* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" d:"); Serial.print(sinkAlarmDuration); Serial.print(" p:"); Serial.println(sinkAlarmPitch);}

    //map values of velocity to pitch
    sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);
    
    // initiate sinkAlarm
    tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); 
  }

  //keep track of the previous beepDuration
  prevDur = beepDuration;
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



