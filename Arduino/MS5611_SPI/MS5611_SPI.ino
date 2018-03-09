#include "MS5611.h"

MS5611_SPI MS5611;

#define MS5611_CSB 13
#define D1_OSR 4
#define D2_OSR 4

float temperatureF = 0;
float pressurePa = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
unsigned long previousMillis = 0;

void setup(){
  Serial.begin(115200);
  MS5611.begin(MS5611_CSB);
}

void loop(){
  samplesThisSec++; //increment each time the loop cycles
  if(millis() - previousMillis >= 1000){ // Update value of samplesPerSec once every second:
    previousMillis=millis();
    samplesPerSec = samplesThisSec;
    samplesThisSec=0; 
    Serial.println(samplesPerSec);  //print debug info
  }
  
  temperatureF = MS5611.getTemperatureF(D2_OSR);
  pressurePa = MS5611.getPressurePa(D1_OSR);

  //Serial.println(temperatureF);
  //Serial.println(pressurePa);
  
}

