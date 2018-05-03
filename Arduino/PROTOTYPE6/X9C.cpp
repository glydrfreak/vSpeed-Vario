/*
MIT License
Copyright (c) 2017 Phil Bowles
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "X9C.h"

void X9C::_deselectAndSave(){
  digitalWrite(_cs,HIGH);             // unselect chip and write current value to NVRAM
}

void X9C::_deselectNoSave(){
  digitalWrite(_inc,LOW);  
  digitalWrite(_cs,HIGH);             // unselect chip
  digitalWrite(_inc,HIGH);            // always leave inc high - makes coding cleaner / easier
}

void X9C::_stepPot(uint8_t amt,uint8_t dir){
  uint8_t cnt=(amt > X9C_MAX) ? X9C_MAX:amt;
  digitalWrite(_ud,dir);              // set direction
  digitalWrite(_cs,LOW);              // select chip
  while(cnt--){
    digitalWrite(_inc,LOW);           // falling pulse triggers wiper change (xN = cnt)
    delayMicroseconds(1);       
    digitalWrite(_inc,HIGH);
    delayMicroseconds(1);
  }
  delayMicroseconds(100);             // let new value settle; (datasheet P7 tIW)
}

void X9C::begin(uint8_t cs,uint8_t inc,uint8_t ud){
		_cs=cs;
    _inc=inc;
    _ud=ud;
    
    pinMode(_cs,OUTPUT);
    pinMode(_inc,OUTPUT);
    pinMode(_ud,OUTPUT);
}

void X9C::setPot(uint8_t pos,bool save){
  _stepPot(X9C_MAX+1,X9C_DOWN);       // crank it back to (beyond!) "zero" (usu. abt 300R for a 100k [104] pot)
  _stepPot(pos,X9C_UP);       	      // put it at abs value of where we want it...
  save ? _deselectAndSave():_deselectNoSave();
}

void X9C::setPotMax(bool save){
  _stepPot(X9C_MAX+1,X9C_UP);         // crank it up to (beyond!) max
  save ? _deselectAndSave():_deselectNoSave();
}
  
void X9C::setPotMin(bool save){
  _stepPot(X9C_MAX+1,X9C_DOWN);       // crank it back to (beyond!) "zero" (usu. abt 300R for a 100k [104] pot)
  save ? _deselectAndSave():_deselectNoSave();  
}
  
void X9C::trimPot(uint8_t amt,uint8_t dir,bool save){
  _stepPot(amt,dir);
  save ? _deselectAndSave():_deselectNoSave();
}

