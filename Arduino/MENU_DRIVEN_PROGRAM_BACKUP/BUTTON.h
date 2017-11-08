#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>



class BUTTON{
 private:
  unsigned long buttonWentHighAt = 0;
  unsigned long buttonWentLowAt = 0;
  
  unsigned long buttDuration = 0;
  int durationToCountAsHold = 500;

  bool lookForRelease = false;
  bool lookForPress = true;
  
 public:
 
  const int NO_ACTION = 0;
  const int CLICK = 1;
  const int HOLD = 2;
  const int PIN = A1;
  /*int PIN = A1;*/
//  constexpr static int NO_ACTION = 0;
//  constexpr static int CLICK = 1;
//  constexpr static int HOLD = 2;

  int PRESS = 0;
  int prevButtonState = 0;
  
  //MAYBE ATTATCH INTERUPT TO THIS FUNCTION; RETURNS 0 IF NO PRESS; 1=CLICK; 2=H0LD;
  int CHECK(int buttonPin, unsigned long currentMillis);

};

#endif
