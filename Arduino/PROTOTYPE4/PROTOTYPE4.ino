//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 
//                                                                              //
//                       v^SPEED VARIO -- PROTOTYPE[4]                          //
/*                        (Vertical Speed Indicator)                            */
/*           Writen by Braedin Butler, with many other contributers             */
//BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|BEEP|


#include "DEFAULT_SETTINGS.h"
#include "MS5611.h"
#include "FILTER.h"
#include "BEEP.h"
#include "OLED.h"
#include "vAdafruit_BluefruitLE_SPI.h"
#include "MICRO_SD.h"
#include "MENU.h"
#include "BUTTON.h"


//THE FOLLOWING LIBRARY CODE HAS BEEN MODIFIED--
//vAdafruit_BluefruitLE_SPI.cpp:   SPISettings bluefruitSPI(/*4000000*/300000, MSBFIRST, SPI_MODE0);
//NOTE-- Unless the code file is changed, TimeoutTimer expires ( tt.expired ), 
//       and some mysterious boxes appear in the BLEUARTRX ble.buffer;


//TODO-- Figure out why the buzzer clicks when the OLED is enabled on the M0. Quieter clicks when OLED disabled;
//TODO-- AMPLIFIER FOR AN ELECTROMAGNETIC BUZZER;
//       FIGURE OUT THE LOUDEST POSSIBLE OPTION WITH THE BLUEFRUT M0;
//       PROTOTYPE2 AND 3 USE A PIEZO BUZZER PS1740;
//       IS ONE OF THE FOLLOWING A GOOD OPTION TO TRY? https://www.digikey.com/short/q724r3
//       CAN I MAKE USE OF THE 400mA FROM THE 3.3V REGULATOR? 
//       BLUEFRUIT M0 IS ONLY ALLOWED RECOMENDED 7mA AND ABSOLUTE 10mA MAX PER PIN;
//TODO-- LEARN THE KALMAN FILTER;
//TODO-- SOFT LATCHING PUSHBUTTON POWER SWITCH;
//       PUSH ON / HOLD OFF;
//       SCHEMATIC IS COMPLETE;
//       USE THE SAME BUTTON FOR TOGGLING A FUTURE SETTINGS MENU;
//TODO-- STORE SETTINGS ON MICRO SD;
//TODO-- GET GOOD AT SURFACE MOUNTING;
//TODO-- SMALLER BATTERY FOR MORE ROOM;
//TODO-- INCREASE LENGTH BY AT LEAST ONE PIN HOLE SO THE OLED FITS BETTER WITH THE BLE MODULE;


#define SD_PIN A2



/*====SERIAL====================================================================*/
#define BAUD_RATE                 115200    // Serial Monitor baud rate

/*====MS5611====================================================================*/
bool ENABLE_MS5611               = true;
byte D1_OSR                         = 5;    // (Default pressure OSR mode 5) 
byte D2_OSR                         = 2;    // (Default temperature OSR mode 2) 
int VELOS_AVGERAGED                = 15;    // Number of most recent velocity values averaged; max is maxVeloData set in MS5611.h
int ACCELS_AVGERAGED               = 5;    // Number of most recent acceleration values averaged; max is maxAccelData set in MS5611.h
int ACCELEROMETER_AVGERAGED        = 15;    // Number of most recent acceleration values averaged; max is maxAccelData set in MS5611.h
#define MS5611_CSB                    13    // Chip/Slave Select Pin

/*====FILTER====================================================================*/
bool ENABLE_FILTER               = true;    // Filter the altitude with a running average of specified duration
int AVERAGING_DURATION           = 1000;    // Greater duration (max 1000 ms) means more samples averaged (see maxDataMemory set in FILTER.h)

/*====BEEP======================================================================*/
#define ALLOW_INTERRUPT             true    // Allows a beep cycle to be updated before completion
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer  

/*====OLED======================================================================*/
#define CHART_WIDGET                true    // Live chart of velocity
#define SCROLLING_ALTITUDE          true    // Custom OLED Widget
#define BATTERY_ICON                true    // Custom OLED Widget
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

/*====BATTERY===================================================================*/
#define VBATPIN                        9    // Pin monitors battery level (Pin A7)

/*====BLUETOOTH=================================================================*/
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended set 4, set to -1 if unused...
#define VERBOSE_MODE               false    // If set to 'true' enables debug output


void SWITCH_BLE_MODE(int bluetoothMode);    // BLE MODULE INITIALIZES ACCORDING TO SPECIFIED BLUETOOTH_MODE;
bool customMode                 = false;    // Custom transmission
bool altiOnly                   = false;
bool veloOnly                   = false;
void receiveCommands();
void transmitVspeed();
void transmitFlySkyHy();

Default SETTING;
MEMORY STORAGE;
MENU MENU;
MS5611_SPI MS5611;
FILTER FILTER;
BEEP BEEP;
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);
vAdafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
float accelFtPerSecPerSec = 0;

float netMetersPerSecPerSec = 0;
float netFeetPerSecPerSecA = 0;
float Aa = 32.174;
float Av = 0;
float Pa = 1;
float Pv = 0;
float Px = 0;
float prevPx = 0;
float prevPv = 0;
float prevPrevPx = 0;
float avgAccelFtPerSecPerSec(float accelerationFtPerSecPerSec, int averageThisMany);
bool changePv = true;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long beepMillis = 0;
unsigned long bleMillis = 0;
long battMillis = -5000;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
int y[64] = {24};          // Used with OLED
int dly = 0;
float batteryLvl = 0;
int batteryPercent = 100;

float getBatteryLvl();
void scrollingAltitude(float scrolledAlti); // Custom OLED Widget
void liveChart(int v);                      // Custom OLED Widget
void batteryIcon(float battLvl);            // Custom OLED Widget


//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 

void setup() {
  //for(int i = 0; i < 24; i++){pinMode(i, OUTPUT);}
  
  Serial.begin(BAUD_RATE);

  if(SD.begin(SD_PIN)){
    SETTING.BLUETOOTH_MODE = STORAGE.findVariable(STORAGE.search_BLUETOOTH_MODE);
  }
  
  ble.begin(VERBOSE_MODE);

  if(ENABLE_MS5611){ MS5611.begin(MS5611_CSB); }
  
  BEEP.begin(BEEP_PIN);
  BEEP.setClimbThreshold(SETTING.CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SETTING.SINK_ALARM_TRIGGER);   //ft/s
  BEEP.setClimbPitchMax(SETTING.CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(SETTING.CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SETTING.SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SETTING.SINK_PITCH_MIN);             //Hz

  if(SETTING.ENABLE_OLED){
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.setFontType(0);

    
    //IF STARTING UP IN ANDROID MODE:
    if(SETTING.BLUETOOTH_MODE==1){
      oled.drawBitmap(SETTING.logo);  // Draw v^SPEED logo
    }

    //IF STARTING UP IN FLYSKYHY MODE:
    else if(SETTING.BLUETOOTH_MODE==2){
      oled.setCursor(2,24);
      oled.print("Flyskyhy");
    }
    
    oled.display();         // Display what's in the buffer (splashscreen)
  }

  SWITCH_BLE_MODE(SETTING.BLUETOOTH_MODE);  //INITIALIZES THE SPECIFIED BLUETOOTH MODE;

}

//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED 

void loop() {
  
  //if needed, slow down the loop (default zero ms)
  delay(dly);   

  //keep track of program runtime in milliseconds
  currentMillis = millis();

  //increment each time the loop cycles
  samplesThisSec++;
  
  // Update value of samplesPerSec once every second:
  if(currentMillis - previousMillis >= 1000){
    previousMillis=currentMillis;
    samplesPerSec = samplesThisSec;
    samplesThisSec=0; 
    //print debug info
    Serial.println(samplesPerSec);
  }
  

  //====BATTERY================================================================/
    //Measure battery level every 5 seconds
    if(SETTING.MEASURE_BATTERY && currentMillis-battMillis>=5000){
      battMillis = currentMillis;
      batteryLvl = getBatteryLvl();
      batteryPercent = (int)(batteryLvl*156.25 - 556.25);
      if(batteryPercent > 99){batteryPercent = 99;}
      else if(batteryPercent < 0){batteryPercent = 0;}
    }
  /*(end BATTERY)*/

  
  //====MS5611=================================================================/
  if(ENABLE_MS5611){  
    
    temperatureF = MS5611.getTemperatureF(D2_OSR);                                                                  //TEMPERATURE
    pressurePa = MS5611.getPressurePa(D1_OSR);                                                                      //PRESSURE
    altitudeFt = MS5611.getAltitudeFt(temperatureF, pressurePa);                                                    //ALTITUDE
    if(ENABLE_FILTER){altitudeFt = FILTER.RUNNING_AVERAGE(altitudeFt, samplesPerSec, AVERAGING_DURATION);}          //FILTER
    velocityFtPerSec = MS5611.getVelocityFtPerSec(altitudeFt, currentMillis, VELOS_AVGERAGED);                      //VERTICAL SPEED      
    if(SETTING.ENABLE_BEEP && currentMillis > 4000){                                                                //BEEP
      if(SETTING.BEEP_TYPE == 1){BEEP.basedOnVelocity(altitudeFt, velocityFtPerSec, currentMillis);}
      if(SETTING.BEEP_TYPE == 2){BEEP.bufferedDurationIncrements(altitudeFt, velocityFtPerSec, currentMillis);}
    }

  }/*(end MS5611)*/


  //====BLE====================================================================/ 
  if(SETTING.ENABLE_BLUETOOTH){
    
    if(SETTING.BLUETOOTH_MODE==1){
      receiveCommands();    //Custom Bluetooth Commands Handled When Received From Mobile Devices
      transmitVspeed();     //Custom Bluetooth Transmissions to Mobile Devices
    }
    
    else if(SETTING.BLUETOOTH_MODE==2){ transmitFlySkyHy(); }
    
  }/*(end BLE)*/

  
  //====OLED===================================================================/ 
  if(SETTING.ENABLE_OLED && currentMillis>2000){  

    switch(MENU.CURRENT_PAGE){
      case MENU.page.FLY:
        oled.clear(PAGE);  //Clear the screen
        if(ENABLE_MS5611){
          if(CHART_WIDGET){liveChart(velocityFtPerSec, SETTING.CHART_SPEED);}
          if(SETTING.MEASURE_BATTERY && BATTERY_ICON){batteryIcon(batteryLvl);}
          if(SCROLLING_ALTITUDE){scrollingAltitude(altitudeFt);}
          if(velocityFtPerSec>=0){oled.setCursor(50,41);}
          else{oled.setCursor(44,41);}
          oled.print(velocityFtPerSec,0);
        }
        else if(!ENABLE_MS5611){oled.line(random(0,64), random(0,48), random(0,64), random(0,48));} 
        break;
      case MENU.page.MAIN:
        //TODO--DISPLAY THE MAIN MENU;
        break;
      case MENU.page.BEEP:
        //TODO--DISPLAY BEEP ICON FOR CHANGING OPTIONS;
        break;
      case MENU.page.VOLUME:
        //TODO--DISPLAY VOLUME ICON FOR CHANGING OPTIONS;
        break;
      //...
        
    }

    oled.display();   //Draw the new screen
    
    
  }/*(end OLED)*/

  
}/*(end loop)*/


//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    


//Custom Widget
void batteryIcon(float battLvl){
  int bars = round((((3 - 0) / (4.2 - 3.5)) * (battLvl - 3.5)) + 0);
  oled.rect(57,0, 6,4);
  oled.rect(58,1, 1+bars,2);
  oled.pixel(63,1);
  oled.pixel(63,2);
}


//Battery Monitor
float getBatteryLvl(){  
  pinMode(VBATPIN, INPUT);
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}



//Custom Widget
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

//Custom Widget
unsigned long chartMillis = 0;
void liveChart(int v, float speed){
  
  //IF ITS TIME TO SHIFT THE LINE CHART
  if(currentMillis-chartMillis >= speed*1000.0/64.0 ){
    chartMillis=currentMillis;
    int p = -2*(v) + 24;
    for(int i = 0; i < 63; i++){
      y[i] = y[i+1];        // Shift all pixels to the left one
      //oled.pixel(i, y[i]);  // Draw all the new pixels except the most recent
      oled.line(i,y[i], i+1,y[i+1]);
    }

    y[63] = p;
    if(y[63]>47){y[63]=47;}
    else if(y[63]<0){y[63]=0;}
  
    //oled.pixel(63, y[63]);  // Draw the most recent pixel
    oled.line(62,y[62], 63,y[63]);

  }

  //OTHERWISE, EVERYTHING STAYS THE SAME
  else{ for(int i = 0; i < 63; i++){oled.line(i,y[i], i+1,y[i+1]);} }
    
  //DRAW THE ZERO AXIS
  for(int i = 3; i < 60; i+=4){ oled.pixel(i, 24); }
}


//Custom Widget
void scrollingAltitude(float scrolledAlti){
  int Position = (((scrolledAlti - (int)scrolledAlti)*50));  //Calculate scrolledAlti's position
  oled.setCursor(2,Position-24);oled.print(scrolledAlti,0);
  oled.setCursor(2,Position+24);oled.print(scrolledAlti-1,0);
  oled.rect(0,22,27,11);
  oled.rectFill(1, 23, 25, 9, 0, 0);
  oled.setCursor(2,24);
  oled.print(scrolledAlti,0);
}


//Custom Bluetooth Commands Handled When Received From Mobile Devices
//CURRENTLY, NO BLUETOOTH COMMANDS CHANGE ANY STORED DEFAULT VALUES;
void receiveCommands(){
  // Check for incoming characters from Mobile Device
  ble.println("AT+BLEUARTRX");
  ble.readline(); //I think this receives something from somewhere and puts it into some buffer..?
  
  if (strcmp(ble.buffer, "OK")) {
    //Serial.print("    [Something is in the buffer]:[");
    String command = ble.buffer;
    //Serial.print(command);Serial.println("]     ");

    if(command == "m"){ENABLE_MS5611 = false;}
    else if(command == "M"){ENABLE_MS5611 = true;}

    if(command == "k"){SETTING.ENABLE_BLUETOOTH = 0;}  // Kill BLE connection
    
    if(command == "V"){SETTING.MEASURE_BATTERY=1;}        // display supply power supply voltage
    else if(command == "v"){SETTING.MEASURE_BATTERY=0;}  // display "0.00V" and don't calculate anything to improve samplesPerSec

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
      SETTING.ENABLE_BEEP = 1;
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
      SETTING.ENABLE_BEEP = 0;       
    }      
    
    if(command == "I"){
      /*Serial.println("  transmit:ALL");*/
      SETTING.MEASURE_BATTERY = 1; 
      customMode = false; 
      altiOnly = false; 
      veloOnly = false;
    }
    if(command == "A" || command == "!B41"){
      /*Serial.println("  transmit:ALTITUDE_ONLY");*/
      SETTING.MEASURE_BATTERY = 0; 
      altiOnly = true; 
      veloOnly = false; 
      customMode = true;
    }
    if(command == "S"){
      /*Serial.println("  transmit:VELOCITY_ONLY");*/
      SETTING.MEASURE_BATTERY = 0; 
      veloOnly = true; 
      altiOnly = false; 
      customMode = true;
    }

    
    if(command == "d"){
      oled.clear(ALL);        // Clear the display's internal memory
      SETTING.ENABLE_OLED = 0;
      //Serial.println("  oled:OFF");
    }
    else if(command == "D"){
      //Serial.println("  oled:ON");
      SETTING.ENABLE_OLED = 1;    
      oled.clear(ALL);        // Clear the display's internal memory
      oled.drawBitmap(SETTING.logo);  // Draw v^SPEED logo
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

    //RECEIVE ACCELEROMETER DATA FROM PHONE
    if(command.startsWith("x")){
      // Example: "x9.81" (net acceleration of 9.81 m/s^2 OR 32.1 ft/s^2)
      String s = command.substring(1);
      float f = s.toFloat();
      netMetersPerSecPerSec = abs(f-9.81);   
      netFeetPerSecPerSecA = netMetersPerSecPerSec*3.281;
      //Serial.print("  netMetersPerSecPerSec:");
      //Serial.println(netMetersPerSecPerSec);
    }

  }
}


bool firstAccel = true;
static const int maxAccelData = 16;
float ACCEL[maxAccelData-1] = {};
float avgAccelFtPerSecPerSec(float accelerationFtPerSecPerSec, int averageThisMany){
  if(firstAccel){
    firstAccel = false;
    return 0;
  }
  else{
    if(averageThisMany >= maxAccelData){averageThisMany = maxAccelData-1;}
    if(averageThisMany < 1){averageThisMany = 1;}
    for(int i = 1; i < averageThisMany; i++){ACCEL[i-1] = ACCEL[i];}  //shift data to make room for more
    ACCEL[averageThisMany-1] = accelerationFtPerSecPerSec;  //add new data
    float sum = 0;
    for(int i = 0; i < averageThisMany; i++){sum += ACCEL[i];} //add all data
    float accel = sum / (float)averageThisMany;  //resulting accel is averaged with the previous maxAccelData# of values
    return accel; 
  }
}


//Custom Bluetooth Transmissions to v^Speed Vario Android App:
void transmitVspeed(){
  if(ENABLE_MS5611){
    ble.print("AT+BLEUARTTX=");  
    if(altiOnly){ble.println(altitudeFt);}
    else if(veloOnly){ble.println(velocityFtPerSec);}
    else{
      ble.print(altitudeFt);
      ble.print("_");     
      ble.print(velocityFtPerSec);  
      ble.print("_");
      if(SETTING.MEASURE_BATTERY){ble.print(batteryLvl);}
      else{ble.print("0");}
      ble.println("V");  //Critical char used for transmission completion indication
    }
  }
  else if(!ENABLE_MS5611){
    ble.print("AT+BLEUARTTX=0_0_");
    if(SETTING.MEASURE_BATTERY){ble.print(batteryLvl);}
    else{ble.print("0");}
    ble.println("V");  //Critical char used for transmission completion indication
  }
}



void transmitFlySkyHy(){

  //EXAMPLE TRANSMISSION SENTENCE:
  //$LK8EX1,98684,99999,-4,28,1100,*02<CR><LF>
 
   /*where:
   $LK8EX1 is keyword
   98684 is filtered pressure in Pa relative to QNH1
   99999 should be altitude relative to QNH but it is ignored when pressure is available
   -4 is filtered vario in cm / s
   28 is temperature in °C
   1100 is battery percentage + 1000 (or 999 during charging)
   *02 is nmea checksum
  <CR><LF> CR and LF characters to terminate the line*/
  
  int cmPerSec = velocityFtPerSec*30.48;
  
  ble.print("AT+GATTCHAR=1,$LK8EX1,");
  ble.println(pressurePa,0);
  
  ble.print("AT+GATTCHAR=1,,99999,"); //ble.print(""); 
  ble.println(cmPerSec);

  ble.print("AT+GATTCHAR=1,,28,10");
  ble.println(batteryPercent);

  ble.print("AT+GATTCHAR=1,");
  ble.println(",*02<CR><LF>");
  
}



void SWITCH_BLE_MODE(int bluetoothMode){
  
  //TO ENABLE FLYSKYHY TRANSMISSION MODE:
  if(bluetoothMode==2){

    //IF YOU WANT TO WAIT FOR THE SERIAL MONITOR
    //while(!Serial){Serial.println(" INITIALISING BLUETOOTH FOR FLYSKYHY TRANSMISSION: ");} 

    //IF PIN MODES ARE THE ISSUE, CHANGE THE SPI PERIPHERAL'S ENABLE PINS TO A KNOWN STATE:
    //for(int i = 0; i < 24; i++){pinMode(i, OUTPUT);}  //EVERY PIN TO AN OUTPUT
    //pinMode(13, OUTPUT); digitalWrite(13, HIGH);      //DISABLE MS5611
    //pinMode(11, OUTPUT); digitalWrite(11, HIGH);      //DISABLE OLED
  
    //TO PERFORM FACTORY RESET:
    if( !ble.factoryReset() ){ Serial.println(" COULD NOT FACTORY RESET "); while(1); }
  
    //TODO -- ADD NECESSARY ADVERTISING DATA:
    ble.sendCommandCheckOK(F( "AT+GATTCLEAR" )); 
    ble.sendCommandCheckOK(F( "AT+GAPSETADVDATA=02-01-06-11-07-1B-C5-D5-A5-02-00-03-A9-E3-11-8B-AA-A0-C6-79-E0" )); 

  
    //TODO -- ADD A BLUETOOTH SERVICE AND CHARACTERISTIC FOR FLYSKYHY COMPATIBILITY: (Same specs as SkyDrop Vario)
    ble.sendCommandCheckOK(F( "AT+GATTADDSERVICE=UUID128=E0-79-C6-A0-AA-8B-11-E3-A9-03-00-02-A5-D5-C5-1B" )); 
    ble.sendCommandCheckOK(F( "AT+GATTADDCHAR=UUID=0xFFE1,PROPERTIES=0x10,MIN_LEN=7,MAX_LEN=20,VALUE=$LK8EX1" )); 
    
  
    //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
    //ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=v^Speed PROTOTYPE[3]" )); ble.reset(); 
    ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=SkyDrop" )); ble.reset(); 
  
    
    //TO SEE WHAT BLE SERVICES AND CHARACTERISTICS ARE CURRENTLY SET:
    ble.sendCommandCheckOK(F( "AT+GATTLIST" ));
    
  }

  //TO ENABLE ANDROID TRANSMISSION MODE:
  else if(bluetoothMode==1){
    
    //IF YOU WANT TO WAIT FOR THE SERIAL MONITOR
    //while(!Serial){Serial.println(" INITIALISING BLUETOOTH FOR ANDROID TRANSMISSION: ");}  

    //TO PERFORM FACTORY RESET:
    if( !ble.factoryReset() ){ Serial.println(" COULD NOT FACTORY RESET "); while(1); }


    //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
    ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=v^Speed PROTOTYPE[3]" )); ble.reset(); 


    //TO SEE WHAT BLE SERVICES AND CHARACTERISTICS ARE CURRENTLY SET:
    ble.sendCommandCheckOK(F( "AT+GATTLIST" ));
  
  }
  
}



//Custom Widget (not used)
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








