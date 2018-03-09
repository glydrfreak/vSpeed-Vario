#include "BUTTON.h"

int BUTTON::CHECK(int buttonPin, unsigned long currentMillis){
  
  int Button = analogRead(buttonPin);
  buttDuration = 0;

  if(prevButtonState!=0 && Button>500){
    return 0;
  }

  if(Button<500){
    prevButtonState = 0;
    return 0;
  }
  
  if(prevButtonState==0 && Button>500){
    howMany++;
    Serial.print("howMany:"); Serial.println(howMany);
    //BUTTON IS PRESSED;
    Serial.print("PRESSED...");
    while(Button>500){
      Button = analogRead(buttonPin);
      delay(1);
      buttDuration++; 
      if(buttDuration>=durationToCountAsHold){
        Serial.println("HOLD");
        prevButtonState=2; 
        return 2;
      }
    }
    Serial.println("CLICK");
    Serial.print("buttDuration:"); Serial.println(buttDuration);
    return 1; 
  }
  
} 

