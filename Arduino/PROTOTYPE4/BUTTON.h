#ifndef BUTTON_H
#define BUTTON_H

class BUTT{
  
 public:
 
  #define BUTTON_NO_ACTION 0
  #define BUTTON_CLICK 1
  #define BUTTON_HOLD 2

  int buttonPress = 0;
  int checkButton(){} //ATTATCH INTERUPT TO THIS FUNCTION; RETURN 0 IF NO PRESS; 1=CLICK; 2=H0LD;

};

#endif
