#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class BUTT{
 private:
  unsigned long buttonWentHighAt = 0;
  unsigned long buttonWentLowAt = 0;
  unsigned long buttDuration = 0;
  bool measureWhenLow = false;
  
 public:
 
  constexpr static int BUTTON_NO_ACTION = 0;
  constexpr static int BUTTON_CLICK = 1;
  constexpr static int BUTTON_HOLD = 2;

  int buttonPress = 0;
  
  //MAYBE ATTATCH INTERUPT TO THIS FUNCTION; RETURNS 0 IF NO PRESS; 1=CLICK; 2=H0LD;
  int checkButton(int buttonPin, unsigned long currentMillis);

};

#endif
