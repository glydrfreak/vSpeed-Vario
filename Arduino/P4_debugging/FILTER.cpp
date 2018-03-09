/**************************************************************************/
/*
    @file     FILTER.cpp
    @author   Braedin Butler
    
    MS5611-01BA03_datasheet:
      http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5611-01BA03&DocType=Data+Sheet&DocLang=English
        
    @section LICENSE
*/
/**************************************************************************/

#include "FILTER.h"

float FILTER1::RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration){
  float filteredData = 0;
  float sum = 0;
  samplesToAverage = samplesPerSec/*(estimated)*/ * averagingDuration/*(milliseconds)*/ / 1000.0;
  //Serial.print(" "); Serial.print(samplesToAverage);
  //if(dbg){Serial.print(" "); Serial.print(samplesToAverage);}
  //if(dbg){Serial.print(" "); Serial.print(f1);}
  
  if(f1){
    previousData = newData;
    f1 = false;  //only do this once...
    //DATA[samplesToAverage] = {};
    for(int i = 0; i < maxDataMemory; i++){
      DATA[i] = newData;   // initialize array
      //if(dbg){Serial.print(" (init)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
    }
    filteredData = newData;
    //if(dbg){Serial.print(" |(sum)N/A...   ");}
  }
  else{ //do this the remaining times...
    for(int i = 1; i < samplesToAverage; i++){DATA[i-1] = DATA[i];} // Shift the DATA to make room for one more          
    if(newData > previousData+30 || newData < previousData-30){
      DATA[samplesToAverage-1] = previousData;  // reject newData if something doesn't seem correct
      /*Serial.print("   *FILTER.newData:_ERROR!_Description:_previousData=");
      Serial.print(previousData);
      Serial.print("_newData=");Serial.print(newData);
      Serial.println("_RollingBackTo=previousData_*");*/
    } 
    else{DATA[samplesToAverage-1] = newData;}         // Updated array full of DATA
    //if(dbg){Serial.print(" "); Serial.print(newData);}
  
  
    for(int i = 0; i < samplesToAverage; i++){
      sum += DATA[i];                  // Add all values
      //if(dbg){Serial.print(" (DATA)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
    }
    //if(dbg){Serial.print(" |(sum)"); Serial.print(sum);}   
    filteredData = (float)(sum / samplesToAverage);           // Divide by number of values
    for(int i = samplesToAverage; i < maxDataMemory; i++){
      /*   Fill in the rest of the unmodified array with the current value so that when samplesToAverage changes,          */
      /*   the result wont jump to some previously expired value that was hangin' around in the DATA memory.               */
      /*   ...because samplesToAverage < maxDataMemory... This should not affect the averaged result, BUT WILL BE SLOWER!  */
      DATA[i] = filteredData;
    }
  }
  //if(dbg){Serial.print(" (filtered)"); Serial.print(filteredData);}
  //if(dbg){Serial.print(" ");}
  previousData = filteredData;
  return filteredData;
}




float FILTER2::RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration){
  float filteredData = 0;
  float sum = 0;
  samplesToAverage = samplesPerSec/*(estimated)*/ * averagingDuration/*(milliseconds)*/ / 1000.0;
  //Serial.print(" "); Serial.print(samplesToAverage);
  //if(dbg){Serial.print(" "); Serial.print(samplesToAverage);}
  //if(dbg){Serial.print(" "); Serial.print(f1);}
  
  if(f1){
    previousData = newData;
    f1 = false;  //only do this once...
    //DATA[samplesToAverage] = {};
    for(int i = 0; i < maxDataMemory; i++){
      DATA[i] = newData;   // initialize array
      //if(dbg){Serial.print(" (init)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
    }
    filteredData = newData;
    //if(dbg){Serial.print(" |(sum)N/A...   ");}
  }
  else{ //do this the remaining times...
    for(int i = 1; i < samplesToAverage; i++){DATA[i-1] = DATA[i];} // Shift the DATA to make room for one more          
    if(newData > previousData+30 || newData < previousData-30){
      DATA[samplesToAverage-1] = previousData;  // reject newData if something doesn't seem correct
      /*Serial.print("   *FILTER.newData:_ERROR!_Description:_previousData=");
      Serial.print(previousData);
      Serial.print("_newData=");Serial.print(newData);
      Serial.println("_RollingBackTo=previousData_*");*/
    } 
    else{DATA[samplesToAverage-1] = newData;}         // Updated array full of DATA
    //if(dbg){Serial.print(" "); Serial.print(newData);}
  
  
    for(int i = 0; i < samplesToAverage; i++){
      sum += DATA[i];                  // Add all values
      //if(dbg){Serial.print(" (DATA)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
    }
    //if(dbg){Serial.print(" |(sum)"); Serial.print(sum);}   
    filteredData = (float)(sum / samplesToAverage);           // Divide by number of values
    for(int i = samplesToAverage; i < maxDataMemory; i++){
      /*   Fill in the rest of the unmodified array with the current value so that when samplesToAverage changes,          */
      /*   the result wont jump to some previously expired value that was hangin' around in the DATA memory.               */
      /*   ...because samplesToAverage < maxDataMemory... This should not affect the averaged result, BUT WILL BE SLOWER!  */
      DATA[i] = filteredData;
    }
  }
  //if(dbg){Serial.print(" (filtered)"); Serial.print(filteredData);}
  //if(dbg){Serial.print(" ");}
  previousData = filteredData;
  return filteredData;
}




float FILTER3::RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration) {
	float filteredData = 0;
	float sum = 0;
	samplesToAverage = samplesPerSec/*(estimated)*/ * averagingDuration/*(milliseconds)*/ / 1000.0;
	//Serial.print(" "); Serial.print(samplesToAverage);
	//if(dbg){Serial.print(" "); Serial.print(samplesToAverage);}
	//if(dbg){Serial.print(" "); Serial.print(f1);}

	if (f1) {
		previousData = newData;
		f1 = false;  //only do this once...
					 //DATA[samplesToAverage] = {};
		for (int i = 0; i < maxDataMemory; i++) {
			DATA[i] = newData;   // initialize array
								 //if(dbg){Serial.print(" (init)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
		}
		filteredData = newData;
		//if(dbg){Serial.print(" |(sum)N/A...   ");}
	}
	else { //do this the remaining times...
		for (int i = 1; i < samplesToAverage; i++) { DATA[i - 1] = DATA[i]; } // Shift the DATA to make room for one more          
		if (newData > previousData + 30 || newData < previousData - 30) {
			DATA[samplesToAverage - 1] = previousData;  // reject newData if something doesn't seem correct
														/*Serial.print("   *FILTER.newData:_ERROR!_Description:_previousData=");
														Serial.print(previousData);
														Serial.print("_newData=");Serial.print(newData);
														Serial.println("_RollingBackTo=previousData_*");*/
		}
		else { DATA[samplesToAverage - 1] = newData; }         // Updated array full of DATA
															   //if(dbg){Serial.print(" "); Serial.print(newData);}


		for (int i = 0; i < samplesToAverage; i++) {
			sum += DATA[i];                  // Add all values
											 //if(dbg){Serial.print(" (DATA)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
		}
		//if(dbg){Serial.print(" |(sum)"); Serial.print(sum);}   
		filteredData = (float)(sum / samplesToAverage);           // Divide by number of values
		for (int i = samplesToAverage; i < maxDataMemory; i++) {
			/*   Fill in the rest of the unmodified array with the current value so that when samplesToAverage changes,          */
			/*   the result wont jump to some previously expired value that was hangin' around in the DATA memory.               */
			/*   ...because samplesToAverage < maxDataMemory... This should not affect the averaged result, BUT WILL BE SLOWER!  */
			DATA[i] = filteredData;
		}
	}
	//if(dbg){Serial.print(" (filtered)"); Serial.print(filteredData);}
	//if(dbg){Serial.print(" ");}
	previousData = filteredData;
	return filteredData;
}




float FILTER4::RUNNING_AVERAGE(float newData, int samplesPerSec, float averagingDuration) {
  float filteredData = 0;
  float sum = 0;
  samplesToAverage = samplesPerSec/*(estimated)*/ * averagingDuration/*(milliseconds)*/ / 1000.0;
  //Serial.print(" "); Serial.print(samplesToAverage);
  //if(dbg){Serial.print(" "); Serial.print(samplesToAverage);}
  //if(dbg){Serial.print(" "); Serial.print(f1);}

  if (f1) {
    previousData = newData;
    f1 = false;  //only do this once...
           //DATA[samplesToAverage] = {};
    for (int i = 0; i < maxDataMemory; i++) {
      DATA[i] = newData;   // initialize array
                 //if(dbg){Serial.print(" (init)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
    }
    filteredData = newData;
    //if(dbg){Serial.print(" |(sum)N/A...   ");}
  }
  else { //do this the remaining times...
    for (int i = 1; i < samplesToAverage; i++) { DATA[i - 1] = DATA[i]; } // Shift the DATA to make room for one more          
    if (newData > previousData + 30 || newData < previousData - 30) {
      DATA[samplesToAverage - 1] = previousData;  // reject newData if something doesn't seem correct
                            /*Serial.print("   *FILTER.newData:_ERROR!_Description:_previousData=");
                            Serial.print(previousData);
                            Serial.print("_newData=");Serial.print(newData);
                            Serial.println("_RollingBackTo=previousData_*");*/
    }
    else { DATA[samplesToAverage - 1] = newData; }         // Updated array full of DATA
                                 //if(dbg){Serial.print(" "); Serial.print(newData);}


    for (int i = 0; i < samplesToAverage; i++) {
      sum += DATA[i];                  // Add all values
                       //if(dbg){Serial.print(" (DATA)["); Serial.print(i);Serial.print("]:");Serial.print(DATA[i]);}
    }
    //if(dbg){Serial.print(" |(sum)"); Serial.print(sum);}   
    filteredData = (float)(sum / samplesToAverage);           // Divide by number of values
    for (int i = samplesToAverage; i < maxDataMemory; i++) {
      /*   Fill in the rest of the unmodified array with the current value so that when samplesToAverage changes,          */
      /*   the result wont jump to some previously expired value that was hangin' around in the DATA memory.               */
      /*   ...because samplesToAverage < maxDataMemory... This should not affect the averaged result, BUT WILL BE SLOWER!  */
      DATA[i] = filteredData;
    }
  }
  //if(dbg){Serial.print(" (filtered)"); Serial.print(filteredData);}
  //if(dbg){Serial.print(" ");}
  previousData = filteredData;
  return filteredData;
}
