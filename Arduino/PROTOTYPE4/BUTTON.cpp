#include "BUTTON.h"

int BUTT::checkButton(int buttonPin, unsigned long currentMillis){
  int Button = analogRead(buttonPin);
  if(Button>500 && !measureWhenLow){
    buttonWentHighAt=currentMillis; 
    measureWhenLow=true;
  }
  if(measureWhenLow && Button<=500){
    measureWhenLow=false; 
    buttonWentLowAt=currentMillis;
    buttDuration=abs(buttonWentHighAt-buttonWentLowAt);
  }
  if(currentMillis-buttonWentHighAt>1000){measureWhenLow=false; return 2;}
  if(buttDuration>0 && buttDuration<1000){return 1;}
  return 0;
} 
