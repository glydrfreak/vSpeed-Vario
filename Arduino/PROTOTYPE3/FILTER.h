/**************************************************************************/
/*
    @file     RUNNING_AVERAGE.h
    @author   Braedin Butler

    v^SPEED Vario for Paragliding:
    
    MS5611-01BA03_datasheet:
      http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5611-01BA03&DocType=Data+Sheet&DocLang=English

    
    @section LICENSE
*/
/**************************************************************************/
#ifndef FILTER_H
#define FILTER_H

#include <Arduino.h>

class FILTER{
 private:
  int samplesToAverage;
  static const int maxDataMemory = 60;   // make this number as small as possible without clipping the quantity of samples to be averaged
  bool f1 = true; 
  
 public:
  float DATA[maxDataMemory] = {};
  bool dbg = false;
	float RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration);
  //float AVERAGE(float newData, int samplesPerSec);
};

#endif



