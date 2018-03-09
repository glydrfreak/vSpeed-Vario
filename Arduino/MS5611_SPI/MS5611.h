/**************************************************************************/
/*
    @file     MS5611.h
    @author   Braedin Butler

    v^SPEED Vario for Paragliding:
      
    
    MS5611-01BA03_datasheet:
      http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5611-01BA03&DocType=Data+Sheet&DocLang=English

    This library is intended to run on the Bluefruit Feather 32u4, 
    but can run on any other hardware SPI mircrocontroller with 3.3V or 5V power/logic; 
    (Arduino Uno, ... the whole shabang...)
    just make sure CSB is set correctly, and reflects your wiring.
    I could not find an optimal SPI library for the MS5611 pressure sensor, so I wrote this library.
    I just needed something as fast as possible for use with the v^SPEED Vario.

    MS5611  | BluefruitFeather32u4
    -----------------------------------------------------------------------------------------------------------
    VCC     | 3V      (SupplyVoltageIn --> It's actually 3.3V, labeled 3V, probably to save board space)
    GND     | GND     (Ground --> Ground)
    SCL     | SCK     (SomethingClock --> SomethingClock)
    SDA     | MOSI    (SomethingData, in I2C, is a two way data line --> For SPI wiring, SDA is SDI/MOSI)
    CSB     | 13      (ChipEnable/SlaveSelect --> Any digital pin you want to enable this device)
    SDO     | MISO    (SomethingDataOut --> MasterInSlaveOut)
    PS      | GND     (ProtocolSelect --> This library only supports the faster SPI, not I2C, so connect to GND)
    
    @section LICENSE
*/
/**************************************************************************/
#ifndef MS5611_H
#define MS5611_H

#include <Arduino.h>
#include <SPI.h>

class MS5611_SPI{
  
 private:
 
  uint32_t SCK_SPEED = 20000000;  // MS5611 rated at 20MHz max
  uint8_t CSB = 13;               // chosen default chip select pin
  
  uint16_t FACTORY_DATA;  // PROM byte 0
  uint16_t C1;  // PROM byte 1 -- Pressure sensitivity 
  uint16_t C2;  // PROM byte 2 -- Pressure offset
  uint16_t C3;  // PROM byte 3 -- Temperature coefficient of pressure sensitivity
  uint16_t C4;  // PROM byte 4 -- Temperature coefficient of pressure offset
  uint16_t C5;  // PROM byte 5 -- Reference temperature
  uint16_t C6;  // PROM byte 6 -- Temperature coefficient of the temperature
  uint16_t CRC; // PROM byte 7 -- cyclic redundancy check???... don't know if used in SPI mode...
  
  uint32_t D1;  // pressure sample (in this case, oversample rate == 4096)
  uint32_t D2;  // temperature sample (in this case, oversample rate == 4096)
  /*
  int32_t dT;   // difference between D2 and reference temperature (C5)
  int32_t TEMP; // actual temperature degreesC
  
  int64_t OFF;  // offset at actual temperature
  int64_t SENS; // sensitivity at actual temperature
  int32_t P;    // temperature compensated pressure mbar
  */
  float dT;   // difference between D2 and reference temperature (C5)
  float TEMP; // actual temperature degreesC
  
  float OFF;  // offset at actual temperature
  float SENS; // sensitivity at actual temperature
  float P;    // temperature compensated pressure mbar

  //float altiFeet = 0;
  //float veloFtPerSec = 0;
  float prevAlti = 0;
  float prevVelo = 0;
  //unsigned long currentVeloMillis = 0;
  //unsigned long currentAccelMillis = 0; 
  unsigned long prevTimeVelo = 0;
  unsigned long prevTimeAccel = 0;
  static const int maxVeloData = 31;
  static const int maxAccelData = 16;
  bool firstTimeVelo = true;
  bool firstTimeAccel = true;

 public: 

  float VELO[maxVeloData-1] = {};
  float ACCEL[maxAccelData-1] = {};
 
  bool dbg = false;
  int dly = 0;
  void debug(bool d_bug);
  
  /*  RESET SEQUENCE  */
  void resetSequence();
  
  /*  Read calibration data (factory calibrated) from PROM */
  void promSequence();  // just calls the following eight functions all together
    void initFactoryDataPROM();
    void initC1PROM();
    void initC2PROM();
    void initC3PROM();
    void initC4PROM();
    void initC5PROM();
    void initC6PROM();
    void initCRCPROM();
 
  /*  Set the clock speed (default is 20MHz)  */
  void setClockSpeed(uint32_t clock_speed);
  
  /*  I think this is a constructor... */
  MS5611_SPI();
   
  /*  Begin using the sensor  */
  bool begin(uint8_t MS5611_CSB);

 
  /*  Main Functions for the User */
  float getTemperatureF(int osrD2);
  float getPressurePa(int osrD1);
  float getTemperatureF();
  float getPressurePa();
  float getAltitudeFt();  // Automatically: calls getTemperatureF() and getPressurePa()
  float getAltitudeFt(float tempF, float pressPa);  // Manually: only if calling both functions every time is not needed.
  float getVelocityFtPerSec(float altitudeFeet, unsigned long currentTimeMillis, int averageThisMany);
  float getVelocityFtPerSec(float altitudeFeet, unsigned long currentTimeMillis);
  float getAccelFtPerSecPerSec(float veloFtPerSec, unsigned long currentTimeMillis, int averageThisMany);

  /*  Read digital pressure and temperature data  */
  void triggerD1();               // default mode: (5) == 8.22ms ADC conversion time in OSR_4096
  void triggerD2();               // default mode: (5) == 8.22ms ADC conversion time in OSR_4096
  void triggerD1(uint8_t mode);   // choose mode: (1, 2, 3, 4, 5)
  void triggerD2(uint8_t mode);   // choose mode: (1, 2, 3, 4, 5)
  void readD1ADC();               // Read Pressure Sample
  void readD2ADC();               // Read Temperature Sample

  /*  Calculate temperature */
  void initdT();
  void initTEMP();

  /*  Calculate temperature compensated pressure  */
  void initOFF();
  void initSENS();
  void initP();

  /*  Write or Read awkward amounts of bytes  */
  uint16_t command8read16(uint8_t command);
  uint32_t command8read24(uint8_t command);
};

#endif




