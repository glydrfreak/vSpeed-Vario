/*
  MS5611 Barometric Pressure & Temperature Sensor. Simple Example
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/czujnik-cisnienia-i-temperatury-ms5611.html
  GIT: https://github.com/jarzebski/Arduino-MS5611
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include "MS5611_I2C.h"

MS5611 ms5611;

double referencePressure;

void setup() 
{
  Serial.begin(115200);

  ms5611.begin();

  ms5611.setOversampling(MS5611_HIGH_RES);
}


void loop()
{
  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();

  // Calculate altitude
  //float absoluteAltitude = ms5611.getAltitude(realPressure);
  //float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);

  Serial.println(realTemperature);
  Serial.println(realPressure);

}
