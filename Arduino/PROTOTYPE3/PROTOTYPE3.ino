#include "MS5611.h"
#include "FILTER.h"
#include "BEEP.h"
#include "OLED.h"
#include <Adafruit_BluefruitLE_SPI.h>


/*====SERIAL====================================================================*/
#define BAUD_RATE                 115200    // Serial Monitor baud rate

/*====MS5611====================================================================*/
bool ENABLE_MS5611               = true;
#define D1_OSR                         5    // (Default 5) 
#define D2_OSR                         2    // (Default 2) 
#define MS5611_CSB                    13    // Chip/Slave Select Pin

/*====FILTER====================================================================*/
bool ENABLE_FILTER               = true;    // (RUNNING AVERAGE) Filter the altitude 
int AVERAGING_DURATION           = 1000;    // (Default 250 millieconds) Don't average too many samples at a time

/*====BEEP======================================================================*/
bool ENABLE_BEEP                  = true;    // Enable or Disable the Buzzer
#define BEEP_PIN                       A5    // (Default A5) Pin connected to buzzer  
#define CLIMB_BEEP_TRIGGER             1.0     // (Default 1.0 ft)
#define SINK_ALARM_TRIGGER            -1.0     // (Default -1.0 ft)
#define CLIMB_PITCH_MAX              500.0     // (Default 900.0 Hz)
#define CLIMB_PITCH_MIN              310.0     // (Default 700.0 Hz)
#define SINK_PITCH_MAX               300.0     // (Default 200.0 milliseconds)
#define SINK_PITCH_MIN               200.0     // (Default 150.0 milliseconds)

/*====OLED======================================================================*/
bool ENABLE_OLED                 = true;    // Enable or Disable the OLED Display
#define DATA_WIDGET                false    // Display Altitude, Velocity, Temperature, BatteryLevel
#define POINTY_WIDGET               true    // Arrow pointing either up or down
#define CHART_WIDGET                true    // Live chart of velocity
#define ALTITUDE_LINES              true
int LINE = 24;
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

/*====BATTERY===================================================================*/
bool DISPLAY_BATTERY             = true;
#define VBATPIN                       9/*A7*/    // Pin monitors battery level

/*====BLUETOOTH=================================================================*/
bool ENABLE_BLE                  = false;    // (Default true)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused
#define BUFSIZE                      128    // Size of the read buffer for incoming data
#define VERBOSE_MODE               false    // If set to 'true' enables debug output
bool iPhoneMode = false;
bool altiOnly = false;
bool veloOnly = false;


MS5611_SPI MS5611;
FILTER FILTER;
BEEP BEEP;
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long velocityMillis = 0;
unsigned long loopMillis = 0;
unsigned long loopWait = 30; //ms
float previousAltitude = 0;
     float v5 = 0;
     float v4 = 0;
     float v3 = 0;
     float v2 = 0;
     float v1 = 0;
     float v5avg = 0;
float velocity = 0;         // (ft/s)
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
  
  Serial.begin(BAUD_RATE);
  
  ENABLE_BLE_MODULE(ENABLE_BLE);

  if(ENABLE_MS5611){MS5611.begin(MS5611_CSB);}
  
  BEEP.begin(BEEP_PIN);
  BEEP.setClimbThreshold(CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SINK_ALARM_TRIGGER);   //ft/s
  BEEP.setClimbPitchMax(CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SINK_PITCH_MIN);             //Hz

  if(ENABLE_OLED){
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.drawBitmap(logo);  // Draw v^SPEED logo
    oled.display();         // Display what's in the buffer (splashscreen)
    oled.setFontType(0);
  }

  //ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=v^SPEED VARIO PROTOTYPE3" ));
  //ble.reset();
}

unsigned long cnt = 0;
void loop() {
  currentMillis = millis();

  //====BATTERY================================================================/
    if(DISPLAY_BATTERY){batteryLvl = getBatteryLvl();}
  /*(end BATTERY)*/
  
  //====MS5611=================================================================/
  if(ENABLE_MS5611){  
    temperatureF = MS5611.getTemperatureF(D2_OSR);
    pressurePa = MS5611.getPressurePa(D1_OSR);
    altitudeFt = MS5611.getAltitudeFt(temperatureF, pressurePa);
    //Serial.print("t=");Serial.println(temperatureF);
    //Serial.print("p=");Serial.println(pressurePa);
    //Serial.print("a=");Serial.print(altitudeFt);
    //Serial.println();
  }    
    samplesPerSec++;
    //Serial.println(cnt++);
    if(currentMillis - previousMillis >= 1000){
      sps = samplesPerSec;
      //Serial.println();
      Serial.print(sps); Serial.println("#  ");
      Serial.print(currentMillis-loopMillis); Serial.print("ms");
      samplesPerSec=0; 
      previousMillis=currentMillis;
    }

  if(ENABLE_MS5611){
    //Serial.println();Serial.print(altitudeFt,2); 
    
    if(ENABLE_FILTER){
      altitudeFt = FILTER.RUNNING_AVERAGE(altitudeFt, sps, AVERAGING_DURATION);
      //Serial.print(" f=");
      //Serial.println(altitudeFt);
      //Serial.print(" s=");Serial.println(sps);
      //Serial.println();
    }
  /*(end MS5611)*/ 
   
  //====BEEP===================================================================/  
    if(ENABLE_BEEP && currentMillis > 3000){BEEP.basedOnAltitude(altitudeFt, currentMillis);}
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
      v5avg = (v5+v4+v3+v2+v1)/5.0;
      //v5avg = velocity;
      //Serial.print(velocity);Serial.print(" ");Serial.print(v5avg);
  /*(end VELOCITY)*/
  }/*(MS5611 completion)*/
  
  //====OLED===================================================================/ 
  if(ENABLE_OLED && currentMillis>2000 && ENABLE_MS5611){  

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
     
    oled.setCursor(2,24);
    oled.print(altitudeFt,0);    
   
  }
  else if(ENABLE_OLED && !ENABLE_MS5611){
    oled.clear(PAGE);     // Clear the screen
    int rx1 = random(0,64);
    int ry1 = random(0,48);
    int rx2 = random(0,64);
    int ry2 = random(0,48);
    oled.line(rx1,ry1,rx2,ry2);
  }
  /*(end OLED)*/
  
  //====BLE====================================================================/ 
    //TODO -- Do things with BLE
  if(ENABLE_BLE){
  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  
  /*==== Commands_to_BLE_via_Mobile_Device ====================*/
  if (strcmp(ble.buffer, "OK")) {
    String text = ble.buffer;

    if(text == "m"){ENABLE_MS5611 = false;}
    else if(text == "M"){ENABLE_MS5611 = true;}

    if(text == "k"){ENABLE_BLE = false;}  // Kill BLE connection
    
    if(text == "V"){DISPLAY_BATTERY=true;}        // display supply power supply voltage
    else if(text == "v"){DISPLAY_BATTERY=false;}  // display "0.00V" and don't calculate anything to improve samplesPerSec


    if(text.startsWith("a")){
      // Example: "a250"
      if(text == "a" || text == "a0"){ENABLE_FILTER = false;}
      else{String s = text.substring(1);
        float f = s.toFloat();
        if(f>1000){f=1000.0;}
        if(f<0){f=0; ENABLE_FILTER = false;}
        AVERAGING_DURATION = f; 
        ENABLE_FILTER = true;
      }
    }


    if(text.startsWith("ct")){
      // Example: "ct2"
      String s = text.substring(2);
      float f = s.toFloat();
      if(f<1){f=1;}
      BEEP.setClimbThreshold(f);       //ft climbed
    }
    
    if(text.startsWith("st")){
      // Example: "st-8"
      String s = text.substring(2);
      float f = s.toFloat();
      BEEP.setSinkAlarmThreshold(f);   //ft/s
    }

    if(text.startsWith("cpx")){
      // Example: "cpx800"
      String s = text.substring(3);
      float f = s.toFloat();
      BEEP.setClimbPitchMax(f);        //Hz
      for(int i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);       
    }

    if(text.startsWith("cpn")){
      // Example: "cpn300"
      String s = text.substring(3);
      float f = s.toFloat();
      BEEP.setClimbPitchMin(f);        //Hz
      for(int i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }

    if(text.startsWith("spx")){
      // Example: "cpx250"
      String s = text.substring(3);
      float f = s.toFloat();
      BEEP.setSinkPitchMax(f);        //Hz
      for(int i = BEEP.sinkPitchMin; i <= BEEP.sinkPitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.sinkPitchMax; i >= BEEP.sinkPitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }

    if(text.startsWith("spn")){
      // Example: "cpn200"
      String s = text.substring(3);
      float f = s.toFloat();
      BEEP.setSinkPitchMin(f);        //Hz
      for(int i = BEEP.sinkPitchMin; i <= BEEP.sinkPitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      for(int i = BEEP.sinkPitchMax; i >= BEEP.sinkPitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);
    }    

   
    if(text == "B"){  // TURN BEEP ON
      ENABLE_BEEP = true;
      for(float i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }
    else if(text == "b"){  // TURN BEEP OFF
      for(float i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);
      ENABLE_BEEP = false;       
    }      
    
    if(text == "I"){DISPLAY_BATTERY = true; iPhoneMode = false; altiOnly = false; veloOnly = false;}
    if(text == "A" || text == "!B41"){DISPLAY_BATTERY = false; altiOnly = true; veloOnly = false; iPhoneMode = true;}
    if(text == "S"){DISPLAY_BATTERY = false; veloOnly = true; altiOnly = false; iPhoneMode = true;}

    
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

  }
  }/*end_commands_to_BLE_via_Mobile_Device*/
  
  //=================================================

  if(ENABLE_BLE && ENABLE_MS5611){
      ble.print("AT+BLEUARTTX=");
      
      if(altiOnly){ble.println(altitudeFt,1);}
      else if(veloOnly){ble.println(v5avg);}
      else{ble.print(altitudeFt);}
      
      if(!iPhoneMode){
        ble.print("_");     
        ble.print(v5avg);  
        ble.print("_");
        if(DISPLAY_BATTERY){ble.print(batteryLvl);}
        else{ble.print("0");}
        ble.println("V");  //Critical char used for transmission completion indication
      }
  }
  else if(ENABLE_BLE && !ENABLE_MS5611){
    ble.print("AT+BLEUARTTX=0_0_");
    if(DISPLAY_BATTERY){ble.print(batteryLvl);}
    else{ble.print("0");}
    ble.println("V");  //Critical char used for transmission completion indication
  }
  //=================================================    
  
   /*(end BLE)*/
 
  
  if(ENABLE_OLED){oled.display();}      // Refresh the display

  if(currentMillis-loopMillis < 1000.0 / sps){
    //Serial.print("    loopMS:");Serial.print(currentMillis-loopMillis);
    Serial.print("!");
  }
  else{Serial.print(".");}
  //if(currentMillis>5000){if(currentMillis-loopMillis < loopWait){delay(1);}}
  loopMillis = currentMillis;
  
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


