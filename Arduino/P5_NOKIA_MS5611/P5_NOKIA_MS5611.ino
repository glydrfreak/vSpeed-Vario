#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"
#include "MS5611.h"
#include "MS5611_I2C.h"
#include "FILTER.h"
#include "BEEP.h"


#define NOKIA_DC 10
#define NOKIA_CS 6
#define NOKIA_RST 11
#define NOKIA_LIGHT 12
Adafruit_PCD8544 display = Adafruit_PCD8544(NOKIA_DC, NOKIA_CS, NOKIA_RST);           //HARDWARE SPI
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!


#define MS5611_CSB                    13    // Chip/Slave Select Pin
#define D1_OSR                         5    // (Default pressure OSR mode 5) 
#define D2_OSR                         2    // (Default temperature OSR mode 2)
#define PRESSURE_FILTER_DURATION    1000    // (AVERAGING DURATION: 1ms to 2000ms)
#define ALTITUDE_FILTER_DURATION    1000    // (AVERAGING DURATION: 1ms to 2000ms)
#define VSPEED_FILTER_DURATION       750    // (AVERAGING DURATION: 1ms to 2000ms)
#define TEMPERATURE_FILTER_DURATION    0    // (AVERAGING DURATION: 1ms to 2000ms)
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer 
#define POT_CS                        A4
#define BUTTON_UP                     A0
#define BUTTON_SELECT                 A1
#define BUTTON_DOWN                   A2



MS5611_SPI MS5611;
MS5611_I2C MS5611_I2C; 
FILTER1 FILTER1;
FILTER2 FILTER2;
FILTER3 FILTER3;
FILTER4 FILTER4;
BEEP BEEP;


float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
unsigned long previousMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
int BRIGHTNESS = 0;   // 0% - 100%
int contrast = 0;
int volume = 55;
int volDelay = 100;   // Volume shall only be changed every 100ms;
unsigned long volMillis = 0;


void adjustVolumeTo(int volLevel);



void setup()   {

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);

  SPI.begin();
  pinMode(POT_CS, OUTPUT);
  digitalWrite(POT_CS, HIGH);
  adjustVolumeTo(volume);  // 55, 71, 102, 255

  
  pinMode(NOKIA_LIGHT, OUTPUT);
  analogWrite(NOKIA_LIGHT, map(BRIGHTNESS, 0,100, 255,0)); //0-255;
  
  Serial.begin(115200);

  display.begin();
  display.setContrast(contrast); //Default 100 in display.begin();

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  //MS5611_I2C.begin();
  //MS5611_I2C.setOversampling(MS5611_HIGH_RES);
  MS5611.begin(MS5611_CSB);

}


void loop() {
  
  
  
  //int ANALOG_LIGHT = map(BRIGHTNESS, 0, 100, 255, 0);
  //analogWrite(NOKIA_LIGHT, ANALOG_LIGHT);
  
  samplesThisSec++; //increment each time the loop cycles
  if(millis() - previousMillis >= 1000){ // Update value of samplesPerSec once every second:
    previousMillis=millis();
    samplesPerSec = samplesThisSec;
    samplesThisSec=0; 
    //Serial.println(samplesPerSec);  //print debug info
    contrast += 10;
    display.setContrast(contrast);
  }

      //====MS5611=================================================================/
        
        //TEMPERATURE:
        if(TEMPERATURE_FILTER_DURATION){
          temperatureF = FILTER4.RUNNING_AVERAGE(
            //(MS5611_I2C.readTemperature()*9/5.0)+32,
            MS5611.getTemperatureF(D2_OSR), 
            samplesPerSec, 
            TEMPERATURE_FILTER_DURATION
          );
          //temperatureF = 77;
        }
        else{
          temperatureF = MS5611.getTemperatureF(D2_OSR);
          //temperatureF = (MS5611_I2C.readTemperature()*9/5.0)+32;
        }
        
        //PRESSURE:
        if(PRESSURE_FILTER_DURATION){
          pressurePa = FILTER1.RUNNING_AVERAGE(
            MS5611.getPressurePa(D1_OSR),  //WHY THE HECK IS THIS INVERTED ALL THE SUDDEN???
            //MS5611_I2C.readPressure(), 
            samplesPerSec, 
            PRESSURE_FILTER_DURATION
          );
        }
        else{
          pressurePa = MS5611.getPressurePa(D1_OSR);  //WHY THE HECK IS THIS INVERTED ALL THE SUDDEN???
          //pressurePa = MS5611_I2C.readPressure();
        }
        
        //ALTITUDE:
        if(ALTITUDE_FILTER_DURATION){
          altitudeFt = FILTER2.RUNNING_AVERAGE(
            MS5611.getAltitudeFt(temperatureF, pressurePa), 
            samplesPerSec, 
            ALTITUDE_FILTER_DURATION
          );
        }
        else{
          altitudeFt = MS5611.getAltitudeFt(temperatureF, pressurePa);
        }

        //VERTICAL SPEED:
        if(millis()>6000){
          if(VSPEED_FILTER_DURATION){
            velocityFtPerSec = FILTER3.RUNNING_AVERAGE(
              MS5611.getVelocityFtPerSec(altitudeFt, millis()), 
              samplesPerSec, 
              VSPEED_FILTER_DURATION
            );
          }
          else{
            velocityFtPerSec = MS5611.getVelocityFtPerSec(altitudeFt, millis());
          }
        }
        else{velocityFtPerSec=0;}

        //DEBUG:
        //Serial.println(temperatureF); 
        //Serial.println(pressurePa);
        //Serial.println(altitudeFt); 
        //Serial.print(" "); 
        //Serial.println(velocityFtPerSec);     



        if(millis()>6000){
          BEEP.basedOnVelocity(altitudeFt, velocityFtPerSec, millis());
        }


  
      //====NOKIA5110=================================================================/

  SPI.begin();  //TODO -- FIND OUT WHY THE LCD WON'T RESPOND WITHOUT SPI.begin() EVERY LOOP;

  display.clearDisplay();

  //TEMPERATURE:
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,40);
  //display.print(samplesPerSec);
  display.print(temperatureF,0);
  display.print("F");

  //VELOCITY:
  display.setTextSize(2);
  if(velocityFtPerSec>=10 || velocityFtPerSec<0){display.setCursor(58,34);}
  else if(velocityFtPerSec>=0){display.setCursor(70,34);}
  if(velocityFtPerSec<=-10){display.setCursor(46,34);}
  //display.setTextColor(BLACK); // 'inverted' text
  display.print(contrast,0);

  //ALTITUDE:
  display.setCursor(0,0); 
  if(altitudeFt>=10000){display.setTextSize(2);}
  else{display.setTextSize(3);}
  //display.setTextColor(WHITE, BLACK);
  display.print(altitudeFt,0);
  
  display.display();
  //delay(1);


      //====BUTTONS=================================================================/

        //Serial.print(" up:"); 
        //Serial.print(analogRead(VOL_UP));
        //Serial.print(" "); 
        //Serial.println(analogRead(VOL_DOWN));
        Serial.print(analogRead(BUTTON_UP)); Serial.print(" ");
        if(analogRead(BUTTON_UP)<100 && millis()-volMillis>=volDelay){
          volMillis = millis();
          volume+=2;
          adjustVolumeTo(volume);
          tone(BEEP_PIN, 400, 100);
        }

        Serial.print(analogRead(BUTTON_DOWN)); Serial.print(" ");
        if(analogRead(BUTTON_DOWN)<100 && millis()-volMillis>=volDelay){
          volMillis = millis();
          volume-=2;
          adjustVolumeTo(volume);
          tone(BEEP_PIN, 400, 100);
        }

        Serial.print(analogRead(BUTTON_SELECT)); Serial.println(" ");
        if(analogRead(BUTTON_SELECT)<100){
          //Serial.println("SELECT");
          tone(BEEP_PIN, 400, 100);
        }
  

}









//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//                            DIGITAL VOLUME CONTROL                            //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void adjustVolumeTo(int volLevel){
  
  // take the SS pin low to select the chip:
  digitalWrite(POT_CS, LOW);
  
  //  send in the address and value via SPI:
  int address = 0;
  SPI.transfer(address);
  SPI.transfer(volLevel);
  
  // take the SS pin high to de-select the chip:
  digitalWrite(POT_CS, HIGH);
}
