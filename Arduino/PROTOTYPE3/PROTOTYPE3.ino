//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                       v^SPEED VARIO -- PROTOTYPE[3]                          //
/*                        (Vertical Speed Indicator)                            */
/*           Writen by Braedin Butler, with many other contributers             */
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 



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


#include "MS5611.h"
#include "FILTER.h"
#include "BEEP.h"
#include "OLED.h"
#include "vAdafruit_BluefruitLE_SPI.h"


//THE FOLLOWING LIBRARY CODE HAS BEEN MODIFIED--
//vAdafruit_BLE.h:                 #define BLE_DEFAULT_TIMEOUT      25/*250*/
//vAdafruit_BluefruitLE_SPI.cpp:   SPISettings bluefruitSPI(/*4000000*/300000, MSBFIRST, SPI_MODE0);
//NOTE-- Unless the code files are changed, TimeoutTimer expires ( tt.expired ), 
//       and some mysterious boxes appear in the BLEUARTRX ble.buffer;
//TODO-- Figure out why the buzzer clicks when the OLED is enabled on the M0. Quieter clicks when OLED disabled;



/*====SERIAL====================================================================*/
#define BAUD_RATE                 115200    // Serial Monitor baud rate

/*====MS5611====================================================================*/
bool ENABLE_MS5611               = true;
byte D1_OSR                         = 5;    // (Default pressure OSR mode 5) 
byte D2_OSR                         = 2;    // (Default temperature OSR mode 2) 
int VELOS_AVGERAGED                 = 5;    // Number of most recent velocity values averaged; max is maxVeloData set in MS5611.h
#define MS5611_CSB                    13    // Chip/Slave Select Pin


/*====FILTER====================================================================*/
bool ENABLE_FILTER               = true;    // (RUNNING AVERAGE) Filter the altitude 
int AVERAGING_DURATION           = 1000;    // Don't average too many samples at a time

/*====BEEP======================================================================*/
bool ENABLE_BEEP                 = true;    // Enable or Disable the Buzzer
#define ALLOW_INTERRUPT             true    // Allows a beep cycle to be updated before completion
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer  
#define CLIMB_BEEP_TRIGGER           1.0    // (Default 1.0 ft)
#define SINK_ALARM_TRIGGER          -1.0    // (Default -1.0 ft/s)
#define CLIMB_PITCH_MAX            500.0    // (Default 500.0 Hz)
#define CLIMB_PITCH_MIN            300.0    // (Default 300.0 Hz)
#define SINK_PITCH_MAX             250.0    // (Default 200.0 Hz)
#define SINK_PITCH_MIN             150.0    // (Default 150.0 Hz)

/*====OLED======================================================================*/
bool ENABLE_OLED                 = true;    // Enable or Disable the OLED Display
#define CHART_WIDGET                true    // Live chart of velocity
#define SCROLLING_ALTITUDE          true    // Custom OLED Widget
#define BATTERY_ICON                true    // Custom OLED Widget
//#define DATA_WIDGET               true    // Display Altitude, Velocity, Temperature, BatteryLevel
//#define POINTY_WIDGET            false    // Arrow pointing either up or down
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

/*====BATTERY===================================================================*/
bool MEASURE_BATTERY             = true;
#define VBATPIN                        9    // Pin monitors battery level (Pin A7)

/*====BLUETOOTH=================================================================*/
bool ENABLE_BLE                  = true;    // (Default true)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended set 4, set to -1 if unused...
#define VERBOSE_MODE               false    // If set to 'true' enables debug output
bool customMode                 = false;
bool altiOnly                   = false;
bool veloOnly                   = false;
void receiveCommands();
void transmitData();


MS5611_SPI MS5611;
FILTER FILTER;
BEEP BEEP;
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);
vAdafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long beepMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
int y[64] = {24};          // Used with OLED
int dly = 0;
float batteryLvl = 0;

float getBatteryLvl();
//void numberData();                        // Custom OLED Widget
//void pointyThing(int v);                  // Custom OLED Widget
void scrollingAltitude(float scrolledAlti); // Custom OLED Widget
void liveChart(int v);                      // Custom OLED Widget
void batteryIcon(float battLvl);            // Custom OLED Widget


//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 

void setup() {
  
  Serial.begin(BAUD_RATE);
  
  ble.begin(VERBOSE_MODE);

  MS5611.begin(MS5611_CSB);
  
  BEEP.begin(BEEP_PIN);
  BEEP.setClimbThreshold(CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SINK_ALARM_TRIGGER);   //ft/s
  BEEP.setClimbPitchMax(CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SINK_PITCH_MIN);             //Hz

  oled.begin();           // Initialize the OLED
  oled.clear(ALL);        // Clear the display's internal memory
  oled.drawBitmap(logo);  // Draw v^SPEED logo
  oled.display();         // Display what's in the buffer (splashscreen)
  oled.setFontType(0);

  //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
  //ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=v^SPEED VARIO PROTOTYPE3" ));
  //ble.reset();
}

//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 

void loop() {
  delay(dly);   // (default zero)
  currentMillis = millis();
  samplesThisSec++;
  // Spit out some info to the serial monitor once every second:
  if(currentMillis - previousMillis >= 1000){
    samplesPerSec = samplesThisSec;
    //Serial.println(samplesPerSec);
    samplesThisSec=0; 
    previousMillis=currentMillis;
  }
  

  //====BATTERY================================================================/
    if(MEASURE_BATTERY){batteryLvl = getBatteryLvl();}
  /*(end BATTERY)*/

  
  //====MS5611=================================================================/
  if(ENABLE_MS5611){  
    temperatureF = MS5611.getTemperatureF(D2_OSR);                                                                  //TEMPERATURE
    pressurePa = MS5611.getPressurePa(D1_OSR);                                                                      //PRESSURE
    altitudeFt = MS5611.getAltitudeFt(temperatureF, pressurePa);                                                    //ALTITUDE
    if(ENABLE_FILTER){altitudeFt = FILTER.RUNNING_AVERAGE(altitudeFt, samplesPerSec, AVERAGING_DURATION);}          //FILTER
    velocityFtPerSec = MS5611.getVelocityFtPerSec(altitudeFt, currentMillis, VELOS_AVGERAGED);                      //v^SPEED
    if(ENABLE_BEEP && currentMillis > 4000){                                                                        //BEEP
      if(!ALLOW_INTERRUPT && currentMillis-beepMillis>BEEP.beepWait){
        beepMillis = currentMillis;
        BEEP.basedOnAltitude(altitudeFt, velocityFtPerSec, currentMillis);
        //BEEP.Smooth(altitudeFt, velocityFtPerSec, currentMillis);
      }
      else if(ALLOW_INTERRUPT){
        BEEP.basedOnAltitude(altitudeFt, velocityFtPerSec, currentMillis);
        //BEEP.Smooth(altitudeFt, velocityFtPerSec, currentMillis);
      }
    }     
  }/*(end MS5611)*/


  //====BLE====================================================================/ 
  if(ENABLE_BLE){
    receiveCommands();
    transmitData();
  }/*(end BLE)*/

  
  //====OLED===================================================================/ 
  if(ENABLE_OLED && currentMillis>2000){  
    if(ENABLE_MS5611){
      oled.clear(PAGE);     // Clear the screen
      //if(DATA_WIDGET){numberData();}
      //if(POINTY_WIDGET){pointyThing(velocityFtPerSec);}
      if(CHART_WIDGET){liveChart(velocityFtPerSec);}
      if(MEASURE_BATTERY && BATTERY_ICON){batteryIcon(batteryLvl);}
      if(SCROLLING_ALTITUDE){scrollingAltitude(altitudeFt);}
      if(velocityFtPerSec>=0){oled.setCursor(50,41);}
      else{oled.setCursor(44,41);}
      oled.print(velocityFtPerSec,0);
    }
    else if(!ENABLE_MS5611){
      oled.clear(PAGE);
      oled.line(random(0,64), random(0,48), random(0,64), random(0,48));
    } 
    oled.display(); // Finally update the display
  }/*(end OLED)*/

  
}/*(end loop)*/


//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    

void batteryIcon(float battLvl){
  int bars = round((((3 - 0) / (4.2 - 3.5)) * (battLvl - 3.5)) + 0);
  oled.rect(57,0, 6,4);
  oled.rect(58,1, 1+bars,2);
  oled.pixel(63,1);
  oled.pixel(63,2);
}


float getBatteryLvl(){  
  pinMode(VBATPIN, INPUT);
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}


/*void pointyThing(int v){    
  int p = -2*(v) + 19;
  //oled.setFontType(0);
  oled.setCursor(58, p); 
  if(v>=0){oled.print("^");}
  else{oled.print("v");}
  oled.setCursor(50,41);oled.print(v);
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


void scrollingAltitude(float scrolledAlti){
  int Position = (((scrolledAlti - (int)scrolledAlti)*50));  //Calculate scrolledAlti's position
  oled.setCursor(2,Position-24);oled.print(scrolledAlti,0);
  oled.setCursor(2,Position+24);oled.print(scrolledAlti-1,0);
  oled.rect(0,22,27,11);
  oled.setCursor(2,24);
  oled.print(scrolledAlti,0);
}


void receiveCommands(){
  // Check for incoming characters from Mobile Device
  ble.println("AT+BLEUARTRX");
  ble.readline(); //I think this receives something from somewhere and puts it into some buffer..?
  
  if (strcmp(ble.buffer, "OK")) {
    //Serial.print("    [Something is in the buffer]:[");
    String command = ble.buffer;
    //Serial.print(command);Serial.println("]     ");

    if(command == "m"){ENABLE_MS5611 = false;/*Serial.println("  sensor:OFF");*/}
    else if(command == "M"){ENABLE_MS5611 = true;/*Serial.println("  sensor:ON");*/}

    if(command == "k"){ENABLE_BLE = false;/*Serial.println("  bluetooth:OFF");*/}  // Kill BLE connection
    
    if(command == "V"){MEASURE_BATTERY=true;/*Serial.println("  battery_monitor:ON");*/}        // display supply power supply voltage
    else if(command == "v"){MEASURE_BATTERY=false;/*Serial.println("  battery_monitor:OFF");*/}  // display "0.00V" and don't calculate anything to improve samplesPerSec

    if(command.startsWith("a")){
      // Example: "a250"
      if(command == "a" || command == "a0"){
        ENABLE_FILTER = false;
        //Serial.println("  averaging:OFF");
      }
      else{
        String s = command.substring(1);
        float f = s.toFloat();
        if(f>1000){f=1000.0;}
        if(f<0){f=0; ENABLE_FILTER = false;}
        AVERAGING_DURATION = f; 
        ENABLE_FILTER = true;
        //Serial.print("  averaging:");
        //Serial.println(AVERAGING_DURATION);
      }
    }


    if(command.startsWith("ct")){
      // Example: "ct2"
      String s = command.substring(2);
      float f = s.toFloat();
      if(f<1){f=1;}
      BEEP.setClimbThreshold(f);       //ft climbed
    }
    
    if(command.startsWith("st")){
      // Example: "st-8"
      String s = command.substring(2);
      float f = s.toFloat();
      BEEP.setSinkAlarmThreshold(f);   //ft/s
    }

    if(command.startsWith("cpx")){
      // Example: "cpx800"
      String s = command.substring(3);
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

    if(command.startsWith("cpn")){
      // Example: "cpn300"
      String s = command.substring(3);
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

    if(command.startsWith("spx")){
      // Example: "cpx250"
      String s = command.substring(3);
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

    if(command.startsWith("spn")){
      // Example: "cpn200"
      String s = command.substring(3);
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

   
    if(command == "B"){  // TURN BEEP ON
      ENABLE_BEEP = true;
      //Serial.println("  beep:ON");
      for(float i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);      
    }
    else if(command == "b"){  // TURN BEEP OFF
      for(float i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_PIN, i);
        delay(5);
      }
      noTone(BEEP_PIN);
      //Serial.println("  beep:OFF");
      ENABLE_BEEP = false;       
    }      
    
    if(command == "I"){
      /*Serial.println("  transmit:ALL");*/
      MEASURE_BATTERY = true; 
      customMode = false; 
      altiOnly = false; 
      veloOnly = false;
    }
    if(command == "A" || command == "!B41"){
      /*Serial.println("  transmit:ALTITUDE_ONLY");*/
      MEASURE_BATTERY = false; 
      altiOnly = true; 
      veloOnly = false; 
      customMode = true;
    }
    if(command == "S"){
      /*Serial.println("  transmit:VELOCITY_ONLY");*/
      MEASURE_BATTERY = false; 
      veloOnly = true; 
      altiOnly = false; 
      customMode = true;
    }

    
    if(command == "d"){
      oled.clear(ALL);        // Clear the display's internal memory
      ENABLE_OLED = false;
      //Serial.println("  oled:OFF");
    }
    else if(command == "D"){
      //Serial.println("  oled:ON");
      ENABLE_OLED = true;    
      oled.clear(ALL);        // Clear the display's internal memory
      oled.drawBitmap(logo);  // Draw v^SPEED logo
      oled.display();         // Display what's in the buffer (splashscreen)
      delay(1000);
    }


    if(command.startsWith("w")){
      // Example: "w20" (ms per loop)
      String s = command.substring(1);
      float f = s.toFloat();
      dly = (int)f;
      /*Serial.print("  delay:");
      Serial.println(dly);*/
    }


    if(command.startsWith("op")){
      // Example: "op5" (pressure OSR mode 1-5)
      String s = command.substring(2);
      float f = s.toFloat();
      D1_OSR = (int)f;
      if(D1_OSR>5){D1_OSR=5;}
      else if(D1_OSR<1){D1_OSR=1;}        
      /*Serial.print("  D1_OSR:");
      Serial.println(D1_OSR);*/
    }
    if(command.startsWith("ot")){
      // Example: "ot5" (temperature OSR mode 1-5)
      String s = command.substring(2);
      float f = s.toFloat();
      D2_OSR = (int)f;
      if(D2_OSR>5){D2_OSR=5;}
      else if(D2_OSR<1){D2_OSR=1;}
      /*Serial.print("  D2_OSR:");
      Serial.println(D2_OSR);*/
    }

  }
}


void transmitData(){
  if(ENABLE_MS5611){
    ble.print("AT+BLEUARTTX=");  
    if(altiOnly){ble.println(altitudeFt);}
    else if(veloOnly){ble.println(velocityFtPerSec);}
    else{
      ble.print(altitudeFt);
      ble.print("_");     
      ble.print(velocityFtPerSec);  
      ble.print("_");
      if(MEASURE_BATTERY){ble.print(batteryLvl);}
      else{ble.print("0");}
      ble.println("V");  //Critical char used for transmission completion indication
    }
  }
  else if(!ENABLE_MS5611){
    ble.print("AT+BLEUARTTX=0_0_");
    if(MEASURE_BATTERY){ble.print(batteryLvl);}
    else{ble.print("0");}
    ble.println("V");  //Critical char used for transmission completion indication
  }
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








