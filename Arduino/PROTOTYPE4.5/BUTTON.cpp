#include "BUTTON.h"

int BUTTON::CHECK(int buttonPin, unsigned long currentMillis){
  int Button = analogRead(buttonPin);
  buttDuration = 0;

  if(prevButtonState!=0 && Button>500){
    return 0;
  }

  if(Button<500){
    prevButtonState = 0;
  }
  
  if(prevButtonState==0 && Button>500){
    //BUTTON IS PRESSED;
    buttonWentLowAt=currentMillis;
    Serial.print("LOW@:");Serial.println(buttonWentLowAt);
    while(Button>500){
      Button = analogRead(buttonPin);
      delay(1);
      buttDuration++; 
      if(buttDuration>=durationToCountAsHold){prevButtonState=2; break;}
      buttonWentHighAt=currentMillis;
    }
    Serial.print("HIGH@:");Serial.println(buttonWentHighAt);
    Serial.println(buttDuration);
    if(buttDuration>=durationToCountAsHold){return 2;}
    else{return 1;} 
  }
  else{return 0;}
 
} 
