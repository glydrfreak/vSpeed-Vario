#include "MS5611_SPI.h"
#include "RUNNING_AVERAGE.h"
#include "Beep.h"
#include <SFE_MicroOLED.h>
//#include "theNightsAvicii.h"
//#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
//#include <Adafruit_BluefruitLE_UART.h>

//#include "BluefruitConfig.h"

/*====SERIAL====================================================================*/
#define BAUD_RATE                 115200    // Serial Monitor baud rate

/*====MS5611====================================================================*/
//#define MS5611_INFO                false    // Show operating stats of the sensor (i.e. samplesThisSec)
#define D1_OSR                         5    // (Default 5) 
#define D2_OSR                         2    // (Default 2) 
#define MS5611_CSB                    13    // Chip/Slave Select Pin
//#define MS5611_DEBUG             false    // (Default false)

/*====FILTER====================================================================*/
bool ENABLE_FILTER               = true;    // (RUNNING AVERAGE) Filter the altitude 
int AVERAGING_DURATION           = 1000;    // (Default 250 millieconds) Don't average too many samples at a time

/*====BEEP======================================================================*/
bool ENABLE_BEEP                 = true;    // Enable or Disable the Buzzer
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer  
//#define CLIMB_BEEP_TRIGGER           1.0    // (Default 1.0 ft)
//#define SINK_ALARM_TRIGGER          -8.0    // (Default -8.0 ft)
//#define SINK_ALARM_DURATION_MIN    500.0    // (Default 500.0 milliseconds)
//#define SINK_ALARM_PITCH           250.0    // (Default 250.0 milliseconds)
//#define CLIMB_PITCH_MAX            600.0    // (Default 800.0 Hz)
//#define CLIMB_PITCH_MIN            400.0    // (Default 300.0 Hz)
//#define CLIMB_DURATION_LONG        500.0    // (Default 50.0 milliseconds)
//#define CLIMB_DURATION_SHORT        50.0    // (Default 500.0 milliseconds)
//#define BEEP_DEBUG               false    // (Default false)

/*====OLED======================================================================*/
bool ENABLE_OLED                 = true;    // Enable or Disable the OLED Display
#define DATA_WIDGET                false    // Display Altitude, Velocity, Temperature, BatteryLevel
#define POINTY_WIDGET               true    // Arrow pointing either up or down
#define CHART_WIDGET                true    // Live chart of velocity
#define ALTITUDE_LINES              true
int LINE = 24;
#define DISPLAY_ALTITUDE            true    // Display Altitude        
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

/*====BATTERY===================================================================*/
bool DISPLAY_BATTERY             = true;
#define VBATPIN                       A9    // Pin monitors battery level

/*====BLUETOOTH=================================================================*/
#define ENABLE_BLE                  true    // (Default true)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused
#define BUFSIZE                      128    // Size of the read buffer for incoming data
#define VERBOSE_MODE               false    // If set to 'true' enables debug output
bool iPhoneMode = false;
bool altiOnly = false;
bool veloOnly = false;

MS5611_SPI MS5611;
RUNNING_AVERAGE RUNNING;
BEEP BEEP;
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
//Avicii Avicii;

float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long velocityMillis = 0;
float previousAltitude = 0;
     int v5 = 0;
     int v4 = 0;
     int v3 = 0;
     int v2 = 0;
     int v1 = 0;
     int v5avg = 0;
int velocity = 0;         // (ft/s)
int samplesPerSec = 0;    // Used for calculating averaging duration
int sps = 0;              // Used for displaying samplesPerSec updated every once second
bool flag1 = true;
float batteryLvl;
int y[64] = {24};         // Used with OLED

void ENABLE_BLE_MODULE(bool enable);
void error(const __FlashStringHelper*err){Serial.println(err); while(1);}   // A small helper
float getBatteryLvl();
//void numberData();        // Custom OLED Widget
void liveChart(int v);         // Custom OLED Widget
void pointyThing(int v);       // Custom OLED Widget


uint8_t logo [] = {
0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0x40, 0x60, 0x60, 0x20, 0x20, 0x80, 0xC0, 0xE0, 0x60, 0x70,
0x30, 0x38, 0x18, 0x18, 0x1C, 0x0C, 0x0C, 0x0C, 0x0E, 0x06, 0x06, 0x46, 0x47, 0x63, 0x63, 0x63,
0x33, 0x33, 0x33, 0x1B, 0x1E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0E, 0x1F, 0x3B, 0x31, 0x30, 0x18, 0x18, 0x0C, 0x0C, 0x06, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0F, 0x7C, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x60, 0x70, 0x38, 0x1C, 0x0C, 0x06, 0x06, 0x03, 0x03, 0x03, 0x03, 0x03,
0x03, 0x03, 0x06, 0x0E, 0x0C, 0x18, 0x38, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x18, 0x0C, 0x0C,
0x06, 0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x3F, 0xF8, 0xC0, 0xE0, 0x60, 0x60, 0x60, 0xC0, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF3, 0xF7, 0xF6, 0xF6, 0xF6, 0xF3, 0xF1,
0xF0, 0xF0, 0xF0, 0x70, 0x30, 0xB0, 0x90, 0x10, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70,
0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0x70,
0x70, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0x70, 0x70, 0x70, 0x70, 0xF0,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x3F, 0x9F, 0xCF, 0xE7,
0xF1, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x3F, 0x7F, 0x78, 0x20, 0x87, 0xDF, 0xFE, 0x3E,
0x0F, 0xC3, 0xF0, 0xF4, 0xF7, 0xF3, 0xF8, 0x3C, 0x0F, 0x43, 0x70, 0x74, 0x77, 0xF7, 0xFF, 0x3F,
0x0F, 0x43, 0x70, 0x74, 0x77, 0xF7, 0xFF, 0x3F, 0x0F, 0x43, 0x30, 0xBC, 0xCF, 0xC2, 0xF0, 0xFD
};

void setup() {
  
  //Serial.begin(BAUD_RATE);
  
  ENABLE_BLE_MODULE(ENABLE_BLE);

  //MS5611.debug(MS5611_DEBUG);
  MS5611.begin(MS5611_CSB);
  
  //BEEP.debug(BEEP_DEBUG);
  BEEP.begin(BEEP_PIN);
  //BEEP.begin(BEEP_PIN, CLIMB_BEEP_TRIGGER, SINK_ALARM_TRIGGER);
  //BEEP.setSinkAlarmDuration(SINK_ALARM_DURATION_MIN);
  //BEEP.setSinkAlarmPitch(SINK_ALARM_PITCH);
  //BEEP.setClimbPitchMax(CLIMB_PITCH_MAX);
  //BEEP.setClimbPitchMin(CLIMB_PITCH_MIN);
  //BEEP.setClimbDurationMax(CLIMB_DURATION_LONG);
  //BEEP.setClimbDurationMin(CLIMB_DURATION_SHORT);

  if(ENABLE_OLED){
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.drawBitmap(logo);  // Draw v^SPEED logo
    oled.display();         // Display what's in the buffer (splashscreen)
    while(millis()<2000);   // Display the v^SPEED Vario logo for a couple seconds
    oled.clear(PAGE);       // Clear the buffer.
  }

  //ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=BlueFly-0B76" ));
  //ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Braedin's v^SPEED VARIO" ));
  //ble.sendCommandCheckOK(F( "AT+BleHIDEn=Off" ));
  //ble.reset();
}

void loop() {
  currentMillis = millis();

  //====BATTERY================================================================/
    if(DISPLAY_BATTERY){batteryLvl = getBatteryLvl();}
  /*(end BATTERY)*/
  
  //====MS5611=================================================================/
    temperatureF = MS5611.getTemperatureF(D2_OSR);
    pressurePa = MS5611.getPressurePa(D1_OSR);
    altitudeFt = MS5611.getAltitudeFt(temperatureF, pressurePa);
        
    samplesPerSec++;
    if(currentMillis - previousMillis >= 1000){
      sps = samplesPerSec;
      samplesPerSec=0; 
      previousMillis=currentMillis;
    }
      
      //Serial.println();Serial.print(altitudeFt,2); 
    
    if(ENABLE_FILTER){
      altitudeFt = RUNNING.AVERAGE(altitudeFt, sps, AVERAGING_DURATION/1000.0);
      //Serial.print(" ");Serial.print(altitudeFt,2);
    }
  /*(end MS5611)*/ 
   
  //====BEEP===================================================================/  
    if(ENABLE_BEEP){BEEP.basedOnAltitude(altitudeFt, currentMillis);}
  /*(end BEEP)*/

  //====VELOCITY===============================================================/  
      velocity = ((altitudeFt - previousAltitude)*1000) / (currentMillis - velocityMillis);
      previousAltitude = altitudeFt;
      velocityMillis = currentMillis;
      v5 = v4;
      v4 = v3;
      v3 = v2;
      v2 = v1;
      v1 = velocity;
      v5avg = (v5+v4+v3+v2+v1)/5;
      //v5avg = velocity;
      //Serial.print(velocity);Serial.print(" ");Serial.print(v5avg);
  /*(end VELOCITY)*/
  
  //====OLED===================================================================/ 
  if(ENABLE_OLED){  

    oled.clear(PAGE);     // Clear the screen
  
    //if(DATA_WIDGET){numberData();}
    if(POINTY_WIDGET){pointyThing(v5avg);}
    if(CHART_WIDGET){liveChart(v5avg);}
        
    LINE = (((altitudeFt - (int)altitudeFt)*50));     // Calculate LINE's position
    if(ALTITUDE_LINES){
      oled.setCursor(2,LINE-24);oled.print(altitudeFt,0);
      oled.setCursor(2,LINE+24);oled.print(altitudeFt-1,0);
      oled.rect(0,22,27,11);
    }
     
    if(DISPLAY_ALTITUDE){
      oled.setFontType(0);
      oled.setCursor(2,24);
      oled.print(altitudeFt,0);    
    }
   
    //oled.display();       // (Refresh the display at the end of the loop)
  }/*(end OLED)*/
  
  //====BLE====================================================================/ 
    //TODO -- Do things with BLE

  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  
  /*==== Commands_to_BLE_via_Mobile_Device ====================*/
  if (strcmp(ble.buffer, "OK")) {
    String text = ble.buffer;
    
    if(text == "V"){DISPLAY_BATTERY=true;}        // display supply power supply voltage
    else if(text == "v"){DISPLAY_BATTERY=false;}  // display "0.00V" and don't calculate anything to improve samplesPerSec


    if(text == "a0"){ENABLE_FILTER = false;}
    if(text == "a100"){AVERAGING_DURATION = 100; ENABLE_FILTER = true;}
    if(text == "a250"){AVERAGING_DURATION = 250; ENABLE_FILTER = true;}
    if(text == "a500"){AVERAGING_DURATION = 500; ENABLE_FILTER = true;}
    //if(text == "a750"){AVERAGING_DURATION = 750; ENABLE_FILTER = true;}
    if(text == "a1000"){AVERAGING_DURATION = 1000; ENABLE_FILTER = true;}

    
    //if(text == "A"){ALTITUDE_LINES = true;}      // Enable ALTITUDE_LINES
    //else if(text == "a"){ALTITUDE_LINES = false;} // Disable ALTITUDE_LINES      
         

    if(text == "B"){  // TURN BEEP ON
      ENABLE_BEEP = true;
      for(int i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }
    else if(text == "b"){  // TURN BEEP OFF
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);
      ENABLE_BEEP = false;       
    }      
    
    if(text == "i"){DISPLAY_BATTERY = true; iPhoneMode = false; altiOnly = false; veloOnly = false;}
    if(text == "a" || text == "!B41"){DISPLAY_BATTERY = false; altiOnly = true; veloOnly = false; iPhoneMode = true;}
    if(text == "s"){DISPLAY_BATTERY = false; veloOnly = true; altiOnly = false; iPhoneMode = true;}

    //if(text == "s"){MS5611_INFO = false;}
    //else if(text == "S"){MS5611_INFO = true;}

    
    if(text == "d"){
      oled.clear(ALL);        // Clear the display's internal memory
      ENABLE_OLED = false;
    }
    else if(text == "D"){
      ENABLE_OLED = true;    
      oled.clear(ALL);        // Clear the display's internal memory
      oled.drawBitmap(logo);  // Draw v^SPEED logo
      oled.display();         // Display what's in the buffer (splashscreen)
      delay(1000);
    }

    /*if(text = "c" && CHART_WIDGET){CHART_WIDGET = false;}
    else if(text = "c" && !CHART_WIDGET){CHART_WIDGET = true;}*/
    
  }/*end_commands_to_BLE_via_Mobile_Device*/
  
  //=================================================

      
      ble.print("AT+BLEUARTTX=");
      
      if(altiOnly){
        //veloOnly=false; iPhoneMode=true; DISPLAY_BATTERY = false;
        ble.println(altitudeFt,0); 
      }
      else if(veloOnly){/*altiOnly=false; iPhoneMode=true; DISPLAY_BATTERY = false;*/ ble.println(v5avg);}
      else /*if(!altiOnly && !veloOnly)*/{/*DISPLAY_BATTERY = true;*/ /*altiOnly=false; veloOnly=false; iPhoneMode=false;*/ ble.print(altitudeFt);}
      
      if(!iPhoneMode){
        ble.print("_");     
        //if(MS5611_INFO){ ble.print(sps);}     
        /*if(MS5611_INFO){*/ ble.print(v5avg);/*}*/    
        ble.print("_");
        if(DISPLAY_BATTERY){ble.print(batteryLvl);}
        else{ble.print("0");}
        ble.println("V");  //Critical char used for transmission completion indication
      }
    
  //=================================================    
  /*(end BLE)*/
  
  if(ENABLE_OLED){oled.display();}      // Refresh the display

}/*(end loop)*/

void ENABLE_BLE_MODULE(bool enable){
  if(!enable){ //if set to false...
    const int bleCS = 8;  
    const int bleIRQ = 7;
    const int bleReset = 4;
    pinMode(bleCS, OUTPUT);
    pinMode(bleIRQ, OUTPUT);
    pinMode(bleReset, OUTPUT);
    digitalWrite(bleCS, HIGH);
  }
  else{
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    pinMode(11, OUTPUT);
    digitalWrite(11, HIGH);    
  //=================================================================/
  //Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  //Serial.println(F("---------------------------------------"));

  //Serial.print(F("Initialising the Bluefruit LE module: "));   // Initialise the module

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  
  //Serial.println( F("OK!") );

  ble.echo(false);    // Disable command echo from Bluefruit

  //Serial.println("Requesting Bluefruit info:");
  ble.info();   // Print Bluefruit information

  //Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  //Serial.println(F("Then Enter characters to send to Bluefruit"));
  //Serial.println(); 

  ble.verbose(false);  // debug info is a little annoying after this point!

  //while (! ble.isConnected()) {delay(500);}   // Wait for connection
  //=================================================================/
  }
}


float getBatteryLvl(){  
  pinMode(VBATPIN, INPUT);
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}

void pointyThing(int v){    
  int p = -2*(v) + 19;
  //oled.setFontType(0);
  oled.setCursor(58, p); 
  if(v>=0){oled.print("^");}
  else{oled.print("v");}
  //oled.setFontType(0);
  //oled.setCursor(58, 24); oled.print("-");
  oled.setCursor(50,41);oled.print(v);
}    

/*void numberData(){
    //ALTITUDE...
    oled.setFontType(1);  // Set font type
    oled.setCursor(10, 0); // Set cursor to top-left
    oled.print(altitudeFt,0);oled.setFontType(0);oled.print("ft");
    oled.setFontType(1);oled.println();
    //VELOCITY...
    oled.setFontType(0);
    if(v5avg>=10 && v5avg<100) {oled.setCursor( 7,16);}
    else if(v5avg>=0 && v5avg<10){oled.setCursor(13,16);}
    else if(v5avg<0) {oled.setCursor( 7,16);}
    else if(v5avg<-10){oled.setCursor(0,16);}
    else {oled.setCursor(0,16);}
    oled.print(v5avg,0);
    oled.setFontType(0);oled.print("ft/s");
    oled.setFontType(1);oled.println();
    //TEMPERATURE...
    oled.setFontType(0);  // Set font type
    oled.print(temperatureF,0);oled.println("F");
    //BATTERY_LEVEL...
    oled.print(batteryLvl);oled.println("V");
}*/

void liveChart(int v){
  int p = -2*(v) + 24;
  for(int i = 0; i < 63; i++){
    y[i] = y[i+1];        // Shift all pixels to the left one
    oled.pixel(i, y[i]);  // Draw all the new pixels except the most recent
  }
  y[63] = p;
  if(y[63]>47){y[63]=47;}
  else if(y[63]<0){y[63]=0;}
  
  oled.pixel(63, y[63]);  // Draw the most recent pixel
  for(int i = 0; i < 60; i+=4){
    oled.pixel(i, 24);
  }
}
