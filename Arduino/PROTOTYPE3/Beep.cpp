/*
	BEEP.cpp
	Variometer Beep Library written by Braedin and Paul Butler
	Function Definitions
*/

#include "BEEP.h"


void BEEP::basedOnVelocity(float currentAltitude, float velo, unsigned long currentTime){
  
  //Prevents audio crashing ...hopefully you'll never need more than 20 beeps per second...
  if(currentTime - beepMillis < 50){return;} 
  
  //OPTION[1] BEEP DURATION DEPENDS VELOCITY
  //beepDuration = (((climbDurationShort - climbDurationLong) / (5.0 - 0.0)) * (velo - 0.0)) + climbDurationLong;  

  //DON'T LET THE DURATION RUN FOR TOO LONG
  //if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}

  //BEEP PITCH DEPENDS ON VELOCITY
  beepPitch = (((pitchMax - pitchMin) / (mostClimb - leastClimb)) * (velo - leastClimb)) + pitchMin;
  leastSink = sinkAlarm;
  mostSink = leastSink-10;
  sinkPitch = (((sinkPitchMin - sinkPitchMax) / (mostSink - leastSink)) * (velo - leastSink)) + sinkPitchMax;
  
  //ALLOW A BEEP IF CLIMBED MORE THAN A CERTAIN AMOUNT
  if(currentAltitude - altitudeTriggerMemory >= verticalTrigger){
    
    //TELL THE BUZZER TO EITHER START OR CONTINUE TO BEEP
    needBeeps = true;
      
    //OPTION[2] CALCULATE BEEP DURATION BASED ON TIME SINCE LAST BEEP
    beepDuration = (((float)currentTime - (float)timeTriggerMemory) / 2.0); 

    //DON'T LET THE DURATION RUN FOR TOO LONG
    if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}     
    
    //RESET THE NEXT CLIMB BEEP TRIGGER REFERENCE POINT
    altitudeTriggerMemory = currentAltitude;

    //USE CURRENT TIME AS NEXT REFERENCE POINT FOR DETERMINING BEEP DURATION
    timeTriggerMemory = currentTime; 
      
  }
  
  //RESET THE NEXT CLIMB TRIGGER'S REFERENCE POINT, AND INITIATE SINK ALARM IF SINKING
  else if(currentAltitude - altitudeTriggerMemory < 0){altitudeTriggerMemory = currentAltitude;}
  
  //IF BEEP SHOULD BE BEEPING, EITHER START OR CONTINUE TO BEEP
  if(needBeeps){tone(buzzerPin, beepPitch, (percentageOfCycleOn+0.5)*beepDuration);}

  //PRINT DEBUG INFO
  //Serial.print("velo:"); Serial.print(velo); Serial.print(" needBeeps:"); Serial.print(needBeeps); Serial.print(" beepDuration:"); Serial.print(beepDuration); Serial.print(" beepPitch:"); Serial.println(beepPitch);

  //LET THE BUZZER KNOW THAT IT HAS BEEPED LONG ENOUGH
  if(currentTime - beepMillis >= beepDuration){needBeeps = false; beepMillis = currentTime;}
  
  //IF SINKING FASTER THAN SPECIFIED, INITIATE SINK ALARM
  if(sinkPitch < 10){sinkPitch = 10;}
  if(velo <= sinkAlarm){tone(buzzerPin, sinkPitch, sinkAlarmDuration);}
    
}


void BEEP::basedOnAltitude(float currentAltitude, float velo, unsigned long currentTime){
  
  //Prevents audio crashing ...hopefully you'll never need more than 20 beeps per second...
  if(currentTime-timeTriggerMemory<50){return;} 
  
	// (DECISION 1) Has there been a positive altitude change great enough to trigger a beep?
	if(currentAltitude - altitudeTriggerMemory >= verticalTrigger){

    //print debug info
	  //if(dbg){Serial.print(" [D1Y] *BEEP* "); Serial.print(" *BEEP* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" ");}
  
		// (DECISION 2) Is the duration of the beep going to be too long?
		if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong){

      //print debug info
      //if(dbg) {Serial.print(" [D2Y] "); Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); Serial.print(" ");}
			
			// Limit the beep duration to a defined maximum
			beepDuration = climbDurationLong; 
      
	  } 
		else{
			
			// Calculate beep duration based on time since last beep
			beepDuration = ((currentTime - timeTriggerMemory) / 2.0); 

      // OR Determine beep duration by mapping the values based on velocity
      //beepDuration = map(velo, verticalTrigger, verticalTrigger+10, climbDurationLong, climbDurationShort);

      //keep beepDuration within bounds if desired
      if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}
      
      //print debug info
      //if(dbg) {Serial.print(" [D2N] "); Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); Serial.print(" ");}
      
		}
		
		// Determine pitch by mapping the values based on beepDuration
    //beepPitch = (((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin;
    
    // OR Determine pitch by mapping the values based on velocity
    beepPitch = (((pitchMax - pitchMin) / (verticalTrigger+10.0 - verticalTrigger)) * (velo - verticalTrigger)) + pitchMin;
    //beepPitch = map(velo, verticalTrigger, verticalTrigger+10, pitchMin, pitchMax);
    
    //keep the beepPitch within bounds if desired
    if(beepPitch<pitchMin){beepPitch=pitchMin;}
    if(beepPitch>pitchMax){beepPitch=pitchMax;}

    //print debug info
    //if(dbg){Serial.print(" d:"); Serial.print(beepDuration); Serial.print(" p:"); Serial.print(beepPitch); Serial.println(" ");}
    
    //adjust the wait time for beginning a new cycle if used
		if(velo<1){beepWait = 2*climbDurationLong;}
		else{beepWait = 2*beepDuration;}
		
		// Activate the beep and make each beep a bit longer if desired
		tone(buzzerPin, beepPitch, (percentageOfCycleOn+0.5)*beepDuration); 

    // Use currentAltitude as the next reference point
		altitudeTriggerMemory = currentAltitude; 

    // Use currentTime as the next reference point
		timeTriggerMemory = currentTime;            
	}
	else{

    //print debug info
    //if(dbg) {Serial.print(" [D1N] ");}
		
		// (DECISION 3) Has there been a negative altitude change great enough to reset the vertical trigger and time trigger?
		if(altitudeTriggerMemory - currentAltitude >= verticalTrigger){
      
      //print debug info
      //if(dbg) {Serial.print(" [D3Y] "); Serial.print(" velo:");Serial.println(velo);}
      
      // Use currentAltitude as the next reference point
      altitudeTriggerMemory = currentAltitude; 

      // Use currentTime as the next reference point
      timeTriggerMemory = currentTime;      
         
    }           
		else{

      //print debug info
      //if(dbg) {Serial.print(" [D3N] ");}
      
		}
	}

  //if we're sinking faster than specified, just blare a sink alarm (replaces DECISION 4)
  if(velo <= sinkAlarm){
    
    //print debug info
    //if(dbg){Serial.print(" *SINK_2* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" d:"); Serial.print(sinkAlarmDuration); Serial.print(" p:"); Serial.println(sinkAlarmPitch);}
    
    //map values of velocity to pitch
    //sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);
    sinkAlarmPitch = ((((float)sinkPitchMin - (float)sinkPitchMax) / ((float)sinkAlarm-8.0 - (float)sinkAlarm)) * ((float)velo - (float)sinkAlarm)) + (float)sinkPitchMax;

    
    // initiate sinkAlarm
    tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm
  }

}



void BEEP::durationIncrements(float currentAltitude, float velo, unsigned long currentTime){
  
  //Prevents audio crashing ...hopefully you'll never need more than 20 beeps per second...
  if(currentTime-timeTriggerMemory<50){return;} 
    
  /* (DECISION 1) Has there been a positive altitude change great enough to trigger a beep and increment the vertical trigger? */
  if(currentAltitude - altitudeTriggerMemory >= verticalTrigger){

    //print debug info
    //if(dbg){Serial.print(" [D1Y] *BEEP* "); Serial.print(" *BEEP* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" trig:");Serial.print(currentAltitude - altitudeTriggerMemory); Serial.print(" ");}
  
    /* (DECISION 2) Is the duration of the beep going to be too long? */
    if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong){
      
      //print debug info
      //if(dbg) {Serial.print(" [D2Y] "); Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); Serial.print(" ");}
      
      // Limit the beep duration
      beepDuration = climbDurationLong; 
      
    } 
    else{ 
      
      // Calculate beep duration based on time since last beep
      //beepDuration = ((currentTime - timeTriggerMemory) / 2.0);

      // OR map beepDuration values based on velocity
      //beepDuration = map(velo, verticalTrigger, verticalTrigger+10, climbDurationLong, climbDurationShort);
      beepDuration = (((climbDurationShort - climbDurationLong) / (5.0 - 0.0)) * (velo - 0.0)) + climbDurationLong;

      //print debug info
      //if(dbg) {Serial.print(" [D2N] "); Serial.print(" t:"); Serial.print(currentTime-timeTriggerMemory); Serial.print(" "); Serial.print(" RAW:");Serial.print(beepDuration);}
      
      // 1.3 == 30% increase
      if(beepDuration > 1.3*prevDur){beepDuration = 1.3*prevDur;}
      
      // 0.7 == 30% decrease
      else if(beepDuration < 0.7*prevDur){beepDuration = 0.7*prevDur;}
      
      //no duration change
      else{beepDuration = prevDur;}

      //if not climbing anymore, reset previous beep duration to maximum
      if(velo <= 0){prevDur = climbDurationLong;}

      //keep beepDuration within bounds if desired
      if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}
      
    }
    
    // Determine pitch by mapping the values based on beepDuration
    beepPitch = (((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin;
    //beepPitch = map(beepDuration, climbDurationLong, climbDurationShort, pitchMin, pitchMax);

    // OR Determine pitch by mapping the values based on velo
    //beepPitch = (((pitchMax - pitchMin) / (verticalTrigger+10.0 - verticalTrigger)) * (velo - verticalTrigger)) + pitchMin;
    //beepPitch = map(velo, verticalTrigger, verticalTrigger+10, pitchMin, pitchMax);

    //keep pitch inside of bounds if desired
    if(beepPitch<pitchMin){beepPitch=pitchMin;}
    if(beepPitch>pitchMax){beepPitch=pitchMax;}

    //print debug info
    //if(dbg){Serial.print(" Pd:"); Serial.print(prevDur); Serial.print(" d:"); Serial.print(beepDuration); Serial.print(" p:"); Serial.print(beepPitch); Serial.println(" ");}

    //adjust the wait time for beginning a new cycle if used
    if(velo<1){beepWait = 2*climbDurationLong;}
    else{beepWait = 2*beepDuration;}
    
    // Activate the beep and make the beep a bit longer if desired
    tone(buzzerPin, beepPitch, (percentageOfCycleOn+0.5)*beepDuration); 

    //if a glitch or jump in data happens, use a new altitude reference point for next time
    if(currentAltitude > verticalTrigger+5 + altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}
    
    //if climbing, increment altitudeTriggerMemory
    else if(currentAltitude > verticalTrigger + altitudeTriggerMemory){altitudeTriggerMemory += verticalTrigger;}

    //if sinking, use a new altitude reference point for next time
    else if(currentAltitude < altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}
    
    // Use currentTime as the next reference point
    timeTriggerMemory = currentTime; 
               
  }
  /* (DECISION 3) Has there been a negative altitude change great enough to reset the vertical trigger and time trigger?  */
  else if(altitudeTriggerMemory - currentAltitude >= verticalTrigger){

    //print debug info
    //if(dbg) {Serial.print(" [D3Y] "); Serial.print(" trig:"); Serial.print(altitudeTriggerMemory - currentAltitude); Serial.print(" velo:");Serial.println(velo);}
      
    //if a glitch or jump in data happens, use a new altitude reference point for next time
    if(currentAltitude > verticalTrigger+5 + altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}  
        
    //if climbing, increment altitudeTriggerMemory
    else if(currentAltitude > verticalTrigger + altitudeTriggerMemory){altitudeTriggerMemory += verticalTrigger;}     
        
    //if sinking, use a new altitude reference point for next time
    else if(currentAltitude < altitudeTriggerMemory){altitudeTriggerMemory = round(currentAltitude);}                 
        
    // Use currentTime as the next reference point
    timeTriggerMemory = currentTime; 
      
  }     
          
  //if we're sinking faster than specified, just blare a sink alarm (replaces DECISION 4)
  if(velo <= sinkAlarm){
    
    //print debug info
    //if(dbg){Serial.print(" *SINK_2* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" d:"); Serial.print(sinkAlarmDuration); Serial.print(" p:"); Serial.println(sinkAlarmPitch);}

    //map values of velocity to pitch
    //sinkAlarmPitch = map(velo, sinkAlarm, sinkAlarm-8, sinkPitchMax, sinkPitchMin);
    sinkAlarmPitch = ((((float)sinkPitchMin - (float)sinkPitchMax) / ((float)sinkAlarm-8.0 - (float)sinkAlarm)) * ((float)velo - (float)sinkAlarm)) + (float)sinkPitchMax;
    
    // initiate sinkAlarm
    tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); 
  }

  //keep track of the previous beepDuration
  prevDur = beepDuration;
}


void BEEP::bufferedDurationIncrements(float currentAltitude, float velo, unsigned long currentTime){
  
  //NOTES
  int note;
  int numberOfNotes = 8;
  float  notes[] = {
    261.626,
    293.665,
    329.628,
    349.228,
    391.995,
    440.000,
    493.883,
    523.251
  };
    
  //IF NEEDED TO PREVENT AUDIO CRASHING
  if(currentTime - beepMillis < 50){return;}

  //TRIGGER A BEEP IF CLIMBED MORE THAN A CERTAIN AMOUNT
  if(currentAltitude - altitudeTriggerMemory >= verticalTrigger){
    
    //KEEP TRACK OF HOW MANY BEEPS ARE WAITING THEIR TURN
    beepsWaitingToBeep++;

    //IF BUFFER FILLS UP, AND BEEPS CAN'T KEEP UP, PLAY THE HIGHEST INDEX BEEP
    if(beepsWaitingToBeep > beepBuffSize){beepsWaitingToBeep = beepBuffSize;}

    //PRINT DEBUG INFO
    //Serial.print(" B:"); Serial.print(beepsWaitingToBeep); Serial.print(" T:"); Serial.print(currentAltitude - altitudeTriggerMemory);    

    //RESET THE NEXT TRIGGER'S REFERENCE POINT
    altitudeTriggerMemory = currentAltitude;

    //BEEP DURATION DEPENDS VELOCITY
    beepDuration = (((climbDurationShort - climbDurationLong) / (5.0 - 0.0)) * (velo - 0.0)) + climbDurationLong;

    //BEEP DURATION IS INCREMENTED OR DECREMENTED BY A PERCENTAGE FROM THE PREVIOUS
    if(beepDuration >= prevDur/0.7){beepDuration = prevDur/0.7; note++;}
    else if(beepDuration <= prevDur*0.7){beepDuration = prevDur*0.7; note--;}
    else{beepDuration = prevDur;}
    
    //DON'T LET THE DURATION RUN FOR TOO LONG
    if(beepDuration > climbDurationLong){beepDuration = climbDurationLong; note = 0;}

    //DON'T LET THE DURATION GET TOO SHORT
    if(beepDuration < climbDurationShort){beepDuration = climbDurationShort; note = numberOfNotes-1;}
    
    //RESET REFERENCE POINT
    prevDur = beepDuration;
    
    //OPTION[1] BEEP PITCH DEPENDS ON VELOCITY
    //beepPitch = (((pitchMax - pitchMin) / (10.0 - 0.0)) * (velo - 0.0)) + pitchMin;

    //OPTION[2] BEEP PITCH DEPENDS ON BEEP DURATION
    beepPitch = (((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin;

    //KEEP THE NOTES WITHIN BOUNDS
    if(note<0){note=0;}
    else if(note>numberOfNotes-1){note=numberOfNotes-1;}
    
    //OPTIONAL[3] ASSIGN THE BEEP PITCH TO A NOTE
    //beepPitch = notes[note];

    //PRINT DEBUG INFO
    //Serial.print(" D:"); Serial.print(beepDuration); Serial.print(" P:"); Serial.print(beepPitch);


//================================================/
// IF BUFFER SIZE IS TEN FOR EXAMPLE,
// INDEX IS ZERO THROUGH NINE
// 0 1 2 3 4 5 6 7 8 9
//(x)x(x)_ _ _ _ _ _ _
//  \   \    ^(EMPTY OR ALREADY PLAYED BEEPS)
//   \  (PLAY THIS BEEP NEXT)
//   (NEW BEEP)    
//================================================/


    //MAKE ROOM FOR THE NEW BEEP IN THE BUFFER
    for(int i = beepBuffSize-1; i >= 0; i--){
      beeps[DURATION][i] = beeps[DURATION][i-1];
      beeps[PITCH][i] = beeps[PITCH][i-1];
    }

    //ADD THE NEW BEEP TO THE BUFFER
    beeps[DURATION][0] = beepDuration; 
    beeps[PITCH][0] = beepPitch;

    //PRINT THE UPDATED BUFFER
    //Serial.print(" + V:"); Serial.print(velo); Serial.print(" B:"); Serial.print(beepsWaitingToBeep); for(int i = 0; i < beepsWaitingToBeep; i++){Serial.print(" (D:"); Serial.print(beeps[DURATION][i]); Serial.print(",P:"); Serial.print(beeps[PITCH][i]); Serial.print(")");} Serial.println();
    
  }

  //RESET THE NEXT TRIGGER'S REFERENCE POINT IF SINKING
  else if(currentAltitude - altitudeTriggerMemory < 0){altitudeTriggerMemory = currentAltitude;}

  //RESET PREVIOUS DURATION IF NOT CLIMBING ANYMORE
  if(velo <= 0){prevDur = climbDurationLong;}
  
  //CERTAIN AMOUNT OF TIME SINCE INITIATING PREVIOUS BEEP MUST ELAPSE BEFORE THE NEXT BEEP IS BEEPED
  int waitOnThisIndex = beepsWaitingToBeep;
  if(waitOnThisIndex > beepBuffSize-1){waitOnThisIndex = beepBuffSize-1;}
  beepWait = (beeps[DURATION][waitOnThisIndex]) * 1.4;

  //IF THERE ARE BEEPS IN THE BUFFER, AND SUFFICIENT TIME ELAPSED, PREPARE TO BEEP
  if(beepsWaitingToBeep > 0 && currentTime - beepMillis >= beepWait){

    //INITIATE A BEEP AT THE BUFFERED PITCH AND DURATION
    tone(buzzerPin, beeps[PITCH][beepsWaitingToBeep-1], (percentageOfCycleOn+0.5)*beeps[DURATION][beepsWaitingToBeep-1]);

    //PRINT WHAT JUST PLAYED
    //Serial.print(" >"); Serial.print(" W:"); Serial.print(currentTime - beepMillis); Serial.print(" (D:"); Serial.print(beeps[DURATION][beepsWaitingToBeep-1]); Serial.print(",P:"); Serial.print(beeps[PITCH][beepsWaitingToBeep-1]); Serial.println(")");

    //TIMESTAMP
    beepMillis = currentTime;

    //NOW THERE IS ONE LESS BEEP IN THE BUFFER
    beepsWaitingToBeep--;

    //PRINT DEBUG INFO
    //Serial.print(" B:"); Serial.print(beepsWaitingToBeep);

    //PRINT THE UPDATED BUFFER
    //Serial.print(" - B:"); Serial.print(beepsWaitingToBeep); for(int i = 0; i < beepsWaitingToBeep; i++){Serial.print(" (D:"); Serial.print(beeps[DURATION][i]); Serial.print(",P:"); Serial.print(beeps[PITCH][i]); Serial.print(")");} Serial.println();

  }

  //IF SINKING FASTER THAN SPECIFIED, PREPARE SINK ALARM
  if(velo <= sinkAlarm){
    
    //PRINT DEBUG INFO
    //if(dbg){Serial.print(" *SINK* "); Serial.print(" velo:");Serial.print(velo); Serial.print(" d:"); Serial.print(sinkAlarmDuration); Serial.print(" p:"); Serial.println(sinkAlarmPitch);}

    //SINK ALARM PITCH IS BASED ON VELOCITY
    sinkAlarmPitch = ((((float)sinkPitchMin - (float)sinkPitchMax) / ((float)sinkAlarm-8.0 - (float)sinkAlarm)) * ((float)velo - (float)sinkAlarm)) + (float)sinkPitchMax;
    
    //INITIATE SINK ALARM
    tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration);
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




