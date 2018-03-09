/**************************************************************************/
/*
    @file     MS5611.cpp
    @author   Braedin Butler
    
    MS5611-01BA03_datasheet:
      http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5611-01BA03&DocType=Data+Sheet&DocLang=English
        
    @section LICENSE
*/
/**************************************************************************/

#include "MS5611.h"

MS5611_SPI::MS5611_SPI(){}

bool MS5611_SPI::begin(uint8_t MS5611_CSB){
  CSB = MS5611_CSB;
  pinMode(CSB, OUTPUT);
  digitalWrite(CSB, HIGH);
  SPI.begin();  
  resetSequence();  //  RESET SEQUENCE
  promSequence();   //  Read factory calibration data from PROM
  return true;
}

void MS5611_SPI::setClockSpeed(uint32_t MS5611_CLKSPEED){
  SCK_SPEED = MS5611_CLKSPEED;
}

void MS5611_SPI::debug(bool d_bug){
  dbg = d_bug;
}

uint16_t MS5611_SPI::command8read16(uint8_t command){
  uint16_t response;
  SPI.beginTransaction(SPISettings(SCK_SPEED,MSBFIRST,SPI_MODE0));
  digitalWrite(CSB, LOW);                 // Enable the MS5611 device   
  SPI.transfer(command);                  // send one byte command    
  response = response | SPI.transfer(0);  // read first byte of response
  response = response << 8;               // bits shifted to make room for next byte
  response = response | SPI.transfer(0);  // read second byte of response
  digitalWrite(CSB, HIGH);                // Disable the MS5611 device 
  SPI.endTransaction();
  if(dbg){Serial.print(response, BIN);Serial.print(" ");Serial.println(response);} 
  return response;
}

uint32_t MS5611_SPI::command8read24(uint8_t command){
  uint32_t response;
  SPI.beginTransaction(SPISettings(SCK_SPEED,MSBFIRST,SPI_MODE0));
    digitalWrite(CSB, LOW);                 // Enable the MS5611 device     
      SPI.transfer(command);                  // send one byte command
      response = response | SPI.transfer(0);  // read first byte of response
      response = response << 8;               // bits shifted to make room for next byte
      response = response | SPI.transfer(0);  // read second byte of response
      response = response << 8;               // bits shifted to make room for next byte
      response = response | SPI.transfer(0);  // read third byte of response
    digitalWrite(CSB, HIGH);                // Disable the MS5611 device 
  SPI.endTransaction(); 
  //if(dbg){Serial.print(response, BIN);Serial.print(" ");Serial.println(response);}  
  return response;
}


void MS5611_SPI::resetSequence(){
  SPI.beginTransaction(SPISettings(SCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CSB, LOW);     // Select the MS5611 device
      SPI.transfer(0x1E);         // send 8 bit command -- Reset sequence ... 0x1E
      delayMicroseconds(2800);    // Wait at least 2.8milliseconds for reset
    digitalWrite(CSB, HIGH);    // Deselect the MS5611 device
  SPI.endTransaction();
}

void MS5611_SPI::promSequence(){
  initFactoryDataPROM();
  initC1PROM();
  initC2PROM();
  initC3PROM();
  initC4PROM(); 
  initC5PROM();
  initC6PROM();
  initCRCPROM();  
}

void MS5611_SPI::initFactoryDataPROM(){
  FACTORY_DATA = command8read16(0xA0);  // FACTORY_DATA now has the 16 bit response from command 0xA0
}
void MS5611_SPI::initC1PROM(){
  C1 = command8read16(0xA2);    // C1 now has the 16 bit response from command 0xA2
}
void MS5611_SPI::initC2PROM(){
  C2 = command8read16(0xA4);    // C2 now has the 16 bit response from command 0xA4
}
void MS5611_SPI::initC3PROM(){
  C3 = command8read16(0xA6);    // C3 now has the 16 bit response from command 0xA6
}
void MS5611_SPI::initC4PROM(){
  C4 = command8read16(0xA8);    // C4 now has the 16 bit response from command 0xA8
}
void MS5611_SPI::initC5PROM(){
  C5 = command8read16(0xAA);    // C5 now has the 16 bit response from command 0xAA
}
void MS5611_SPI::initC6PROM(){  
  C6 = command8read16(0xAC);    // C6 now has the 16 bit response from command 0xAC
}
void MS5611_SPI::initCRCPROM(){  
  CRC = command8read16(0xAE);   // CRS now has the 16 bit response from command 0xAE
}

  /* TODO ===================================================================*
    unsigned int C[8];                      // calibration coefficients
    unsigned char n_crc;                    // crc value of the prom

    cmd_reset();                              // reset the module after powerup
    for (i=0;i<8;i++){ C[i]=cmd_prom(i);}   // read calibration coefficients
    n_crc=crc4(C);     

    
    unsigned char crc4(unsigned int n_prom[])
    { 
      int cnt;                              // simple counter
      unsigned int n_rem;                   // crc reminder
      unsigned int crc_read;                // original value of the crc
      unsigned char n_bit;
      n_rem = 0x00;
      crc_read=n_prom[7];                   //save read CRC
      n_prom[7]=(0xFF00 & (n_prom[7]));     //CRC byte is replaced by 0
      for (cnt = 0; cnt < 16; cnt++)        // operation is performed on bytes
      { // choose LSB or MSB
        if (cnt%2==1) n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);
        else n_rem ^= (unsigned short) (n_prom[cnt>>1]>>8);
        for (n_bit = 8; n_bit > 0; n_bit--)
        {
          if (n_rem & (0x8000))
          {
            n_rem = (n_rem << 1) ^ 0x3000;
          }
          else
          {
            n_rem = (n_rem << 1);
          }
        }
      }
      n_rem= (0x000F & (n_rem >> 12));      // // final 4-bit reminder is CRC code
      n_prom[7]=crc_read;                   // restore the crc_read to its original place
      return (n_rem ^ 0x00);
    }
  /*=========================================================================*/

//====================================================================================/
//====================================================================================/


/*  Read digital pressure and temperature data  */
void MS5611_SPI::triggerD1(){
  // DEFAULT MODE: 5 == (OSR_4096 @ 8.22ms)
  SPI.beginTransaction(SPISettings(SCK_SPEED,MSBFIRST,SPI_MODE0));
    digitalWrite(CSB, LOW);     // Enable the MS5611 device
      SPI.transfer(0x48);       // send command to trigger pressure sample (OSR=4096)
    digitalWrite(CSB, HIGH);    // Disable the MS5611 device
  SPI.endTransaction();     
  delayMicroseconds(8220);  // wait at least 8.22ms; (if commented out, do this manually in main program)
}

void MS5611_SPI::triggerD2(){
  // DEFAULT MODE: 2 == (OSR_512 @ 1.06ms)
  SPI.beginTransaction(SPISettings(SCK_SPEED,MSBFIRST,SPI_MODE0));
    digitalWrite(CSB, LOW);     // Enable the MS5611 device
      SPI.transfer(0x52);         // send command to trigger temperature sample (OSR=512)
    digitalWrite(CSB, HIGH);    // Disable the MS5611 device  
  SPI.endTransaction();
  delayMicroseconds(1060);  // wait at least 1.06ms; (if commented out, do this manually in main program) 
}

void MS5611_SPI::triggerD1(uint8_t mode){
  byte OSR;     // OverSampling Ratio
  int DLY;  // Delay (microseconds)
  // CHOOSE MODE:
  switch(mode){
    
    case 1://(OSR_256 @ 0.54ms)
      OSR = 0x40;
      DLY = 540;
      break;
    
    case 2://(OSR_512 @ 1.06ms)
      OSR = 0x42;
      DLY = 1060;
      break;
    
    case 3://(OSR_1024 @ 2.08ms)
      OSR = 0x44;
      DLY = 2080;
      break;
    
    case 4://(OSR_2048 @ 4.13ms)
      OSR = 0x46;
      DLY = 4130;
      break;

    //case 5: (the default case)
    
    default://(OSR_4096 @ 8.22ms)
      OSR = 0x48;
      DLY = 8220;
      break;
  }
  SPI.beginTransaction(SPISettings(SCK_SPEED,MSBFIRST,SPI_MODE0));
    digitalWrite(CSB, LOW);     // Enable the MS5611 device
      SPI.transfer(OSR);        // send command to trigger pressure sample
    digitalWrite(CSB, HIGH);    // Disable the MS5611 device
  SPI.endTransaction();     
  delayMicroseconds(DLY);         // wait for ADC conversion (if commented out, do this manually in main program)
  //delay(dly);
}

void MS5611_SPI::triggerD2(uint8_t mode){
  byte OSR;     // OverSampling Ratio
  int DLY;  // Delay (microseconds)
  // CHOOSE MODE:
  switch(mode){
    
    case 1://(OSR_256 @ 0.54ms)
      OSR = 0x50;
      DLY = 540;
      break;
    
    //case 2: (the default case)
    
    case 3://(OSR_1024 @ 2.08ms)
      OSR = 0x54;
      DLY = 2080;
      break;
    
    case 4://(OSR_2048 @ 4.13ms)
      OSR = 0x56;
      DLY = 4130;
      break;
    
    case 5://(OSR_4096 @ 8.22ms)
      OSR = 0x58;
      DLY = 8220;
      break;
      
    default://(OSR_512 @ 1.06ms)
      OSR = 0x52;
      DLY = 1060;
      break;
  }
  SPI.beginTransaction(SPISettings(SCK_SPEED,MSBFIRST,SPI_MODE0));
    digitalWrite(CSB, LOW);     // Enable the MS5611 device
      SPI.transfer(OSR);        // send command to trigger pressure sample
    digitalWrite(CSB, HIGH);    // Disable the MS5611 device
  SPI.endTransaction();     
  delayMicroseconds(DLY);       // wait for ADC conversion (if commented out, do this manually in main program)
  //delay(dly);
}

void MS5611_SPI::readD1ADC(){
  D1 = command8read24(0x00);  // Read Pressure Sample
  //Serial.print("d1=");Serial.println(D1);
}
void MS5611_SPI::readD2ADC(){
  D2 = command8read24(0x00);  // Read Temperature Sample
  //Serial.print("d2=");Serial.println(D2);
}


/*  Calculate temperature */
void MS5611_SPI::initdT(){
  dT = D2 - C5 * pow(2,8);
  //delayMicroseconds(dly);
}
void MS5611_SPI::initTEMP(){
  TEMP = 2000 + dT * C6 / pow(2,23);    // degreesC * 100 
  //delayMicroseconds(dly);
}

/*  Calculate temperature compensated pressure  */
void MS5611_SPI::initOFF(){
  OFF = C2 * pow(2,16) + (C4 * dT) / pow(2,7); 
  //delayMicroseconds(dly);
}
void MS5611_SPI::initSENS(){
  SENS = C1 * pow(2,15) + (C3 * dT ) / pow(2,8);
  //delayMicroseconds(dly); 
}
void MS5611_SPI::initP(){
  P = (D1 * SENS / pow(2,21) - OFF) / pow(2,15);    // Pa
  //delayMicroseconds(dly);
}

//==== MAIN FUNCTIONS FOR THE USER =====================/
float MS5611_SPI::getTemperatureF(int osrD2){
  SPI.beginTransaction(SPISettings(SCK_SPEED, MSBFIRST, SPI_MODE0)); // Initialize settings for SPI bus
  triggerD2(osrD2);         // Trigger Temperature Sample (Default OSR Mode 2)
  readD2ADC();              // Read the results from ADC conversion
  SPI.endTransaction();     // SPI bus is now ready for other transfer settings
  initdT();                 // calculations
  initTEMP();               // calculations... TEMP == degreesC * 100
  float TemperatureF = (TEMP / 100.00) * (9.0/5.0) + 32;
  //return (float)TEMP / 100.00;   // degreesC
  return TemperatureF;           // degreesF
}

float MS5611_SPI::getPressurePa(int osrD1){
  SPI.beginTransaction(SPISettings(SCK_SPEED, MSBFIRST, SPI_MODE0)); // Initialize settings for SPI bus
  triggerD1(osrD1);         // Trigger Pressure Sample (Default OSR Mode 5)
  readD1ADC();              // Read the results from ADC conversion
  SPI.endTransaction();     // SPI bus is now ready for other transfer settings
  initdT();                 // calculations
  initOFF();                // calculations
  initSENS();               // calculations
  initP();                  // calculations... P == (...)
  return (float)P;          // temperature compensated pressure Pa
}

float MS5611_SPI::getTemperatureF(){
  SPI.beginTransaction(SPISettings(SCK_SPEED, MSBFIRST, SPI_MODE0)); // Initialize settings for SPI bus
  triggerD2(2);             // Trigger Temperature Sample (in default OSR mode 2)
  readD2ADC();              // Read the results from ADC conversion
  SPI.endTransaction();     // SPI bus is now ready for other transfer settings
  initdT();                 // calculations
  initTEMP();               // calculations... TEMP == degreesC * 100
  float TemperatureF = (TEMP / 100.00) * (9.0/5.0) + 32;
  //return (float)TEMP / 100.00;   // degreesC
  return TemperatureF;           // degreesF
}

float MS5611_SPI::getPressurePa(){
  SPI.beginTransaction(SPISettings(SCK_SPEED, MSBFIRST, SPI_MODE0)); // Initialize settings for SPI bus
  triggerD1(5);             // Trigger Pressure Sample (in default OSR mode 5)
  readD1ADC();              // Read the results from ADC conversion
  SPI.endTransaction();     // SPI bus is now ready for other transfer settings
  initdT();                 // calculations
  initOFF();                // calculations
  initSENS();               // calculations
  initP();                  // calculations... P == (...)
  return (float)P;          // temperature compensated pressure Pa
}

float MS5611_SPI::getAltitudeFt(float tempF, float pressPa){
  //float sLvl = 101325.00;
  float sLvl = 100680.00;
  return 504.745*((5.00*(tempF - 32.00))/9.00 + 273.15)*(pow((sLvl/pressPa),0.190223) - 1.00);
}

float MS5611_SPI::getAltitudeFt(){
  float tempF = getTemperatureF();
  float pressPa = getPressurePa();
  float sLvl = 101325.00;
  return 504.745*((5.00*(tempF - 32.00))/9.00 + 273.15)*(pow((sLvl/pressPa),0.190223) - 1.00);
}


float MS5611_SPI::getVelocityFtPerSec(float altiFeet, unsigned long currentVeloMillis, int averageThisMany){
  
  if(firstTimeVelo){
    firstTimeVelo = false;
    prevAlti = altiFeet; //initializing
    prevTimeVelo = currentVeloMillis; //initializing
    return 0;
  }
  else{
    if(averageThisMany >= maxVeloData){averageThisMany = maxVeloData-1;}
    if(averageThisMany < 1){averageThisMany = 1;}
    
    for(int i = 1; i < averageThisMany; i++){VELO[i-1] = VELO[i]; /*Serial.print(VELO[i-1]); Serial.print(" + ");*/}  //shift data to make room for more
    VELO[averageThisMany-1] = (1000.0*((float)altiFeet - (float)prevAlti)) / ((float)currentVeloMillis - (float)prevTimeVelo);  //add new data
    //Serial.print(VELO[averageThisMany-1]); Serial.print(" = ");
    
    prevAlti = altiFeet;
    prevTimeVelo = currentVeloMillis;
    double vsum = 0;
    for(int i = 0; i < averageThisMany; i++){vsum += VELO[i];} //add all data
    float velo = vsum / (float)averageThisMany;  //resulting velo is an average of all averageThisMany# of values
    //Serial.print(vsum); Serial.print(" / "); Serial.print(averageThisMany);
    //Serial.print(" = ["); Serial.print(velo); Serial.println("]");
    
    return velo; 
  }
}



float MS5611_SPI::getVelocityFtPerSec(float altiFeet, unsigned long currentVeloMillis){
  if(firstTimeVelo){
    firstTimeVelo = false;
    prevAlti = altiFeet; //initializing
    prevTimeVelo = currentVeloMillis; //initializing
    return 0;
  }
  else{
    float velo = (1000.0*((float)altiFeet - (float)prevAlti)) / ((float)currentVeloMillis - (float)prevTimeVelo);
    prevAlti = altiFeet;
    prevTimeVelo = currentVeloMillis;
    return velo; 
  }
}


float MS5611_SPI::getAccelFtPerSecPerSec(float veloFtPerSec, unsigned long currentAccelMillis, int averageThisMany){
  if(firstTimeAccel){
    firstTimeAccel = false;
    prevVelo = veloFtPerSec; //initializing
    prevTimeAccel = currentAccelMillis; //initializing
    return 0;
  }
  else{
    if(averageThisMany >= maxAccelData){averageThisMany = maxAccelData-1;}
    if(averageThisMany < 1){averageThisMany = 1;}
    for(int i = 1; i < averageThisMany; i++){ACCEL[i-1] = ACCEL[i];}  //shift data to make room for more
    ACCEL[averageThisMany-1] = abs((1000.0*((float)veloFtPerSec - (float)prevVelo)) / ((float)currentAccelMillis - (float)prevTimeAccel));  //add new data
    float sum = 0;
    for(int i = 0; i < averageThisMany; i++){sum += ACCEL[i];} //add all data
    float accel = sum / (float)averageThisMany;  //resulting accel is averaged with the previous maxAccelData# of values
    //Serial.println(accel);
    prevVelo = veloFtPerSec;
    prevTimeAccel = currentAccelMillis;
    return accel; 
  }
}
//=====================================================/



/*  ADAFRUIT'S READ SPI FUNCTIONS:
uint16_t command8read16(byte reg){
  uint16_t value;
  SPI.transfer(reg | 0x80); // read, bit 7 high
  value = (SPI.transfer(0) << 8) | SPI.transfer(0);
  //delayMicroseconds(20);
  return value;
}

uint32_t command8read24(byte reg){
  uint32_t value;
  SPI.transfer(reg | 0x80); // read, bit 7 high
  value = SPI.transfer(0);
  delayMicroseconds(20);
  value <<= 8;
  value |= SPI.transfer(0);
  delayMicroseconds(20);
  value <<= 8;
  value |= SPI.transfer(0);
  delayMicroseconds(20);
  return value;
}
*/




