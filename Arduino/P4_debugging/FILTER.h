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




class FILTER1{//PRESSURE:
 private:
  int samplesToAverage;
  static const int maxDataMemory = 200;   // make this number as small as possible without clipping the quantity of samples to be averaged
  bool f1 = true; 
  float previousData = 0;
  
 public:
  //int AVERAGING_DURATION = 1000;     // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)
  float DATA[maxDataMemory] = {};
  bool dbg = false;
	float RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration);
};




class FILTER2{//ALTITUDE:
 private:
  int samplesToAverage;
  static const int maxDataMemory = 200;   // make this number as small as possible without clipping the quantity of samples to be averaged
  bool f1 = true; 
  float previousData = 0;
  
 public:
  //int AVERAGING_DURATION = 1000;     // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)
  float DATA[maxDataMemory] = {};
  bool dbg = false;
  float RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration);
};




class FILTER3 {//VELOCITY:
private:
	int samplesToAverage;
	static const int maxDataMemory = 200;   // make this number as small as possible without clipping the quantity of samples to be averaged
	bool f1 = true;
	float previousData = 0;

public:
	//int AVERAGING_DURATION = 1000;     // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)
	float DATA[maxDataMemory] = {};
	bool dbg = false;
	float RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration);
};




class FILTER4 {//TEMPERATURE:
private:
  int samplesToAverage;
  static const int maxDataMemory = 200;   // make this number as small as possible without clipping the quantity of samples to be averaged
  bool f1 = true;
  float previousData = 0;

public:
  //int AVERAGING_DURATION = 1000;     // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)
  float DATA[maxDataMemory] = {};
  bool dbg = false;
  float RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration);
};

#endif




