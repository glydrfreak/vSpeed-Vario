#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>



class BUTTON{
 private:
  
  unsigned long buttDuration = 0;
  int durationToCountAsHold = 250;
  int prevButtonState = 0;
  int howMany=0;
  
 public:
    
  const int NO_ACTION = 0;
  const int CLICK = 1;
  const int HOLD = 2;
  const int PIN = A1;

  int PRESS = 0;
  
  //MAYBE ATTATCH INTERUPT TO THIS FUNCTION; RETURNS 0 IF NO PRESS; 1=CLICK; 2=H0LD;
  int CHECK(int buttonPin, unsigned long currentMillis);

};

#endif
