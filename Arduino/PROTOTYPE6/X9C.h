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
#ifndef X9C_H
#define X9C_H
//
// direction "sense" is set between Vh/Rh (pin 3) and Vw/Rw (pin 5) i.e. with x9c104,
//		setPotMin will give abt 220R - 330R between these pins
//      setPotMax should give abt 100k
//
#include <Arduino.h>

#define X9C_UP LOW
#define X9C_DOWN HIGH
#define X9C_MAX 31
//
// stepPot explicilty does NOT save to NVRAM - allows reboot to old NVRAM value, with minor runtime tweaks
//
class X9C {
	public:
		X9C(){};
		~X9C(){};
		void begin(uint8_t cs,uint8_t inc,uint8_t ud);
		void setPot(uint8_t pos,bool save=true);
		void setPotMax(bool save=true);
	  	void setPotMin(bool save=true);
		void trimPot(uint8_t amt,uint8_t dir,bool save=true);
	private:
		uint8_t _cs, _inc, _ud;
		
		void _deselectAndSave();
		void _deselectNoSave();
		void _stepPot(uint8_t amt,uint8_t dir);
	};

#endif // X9C_H
