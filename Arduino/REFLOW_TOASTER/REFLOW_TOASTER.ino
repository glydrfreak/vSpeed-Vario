#include <SPI.h>
#include "MAX6675.h"
#include "FILTER.h"
#include <Adafruit_GFX.h>
#include "Adafruit_PCD8544.h"


#define relayPin      3
#define thermoCS      2
#define buttonPin     A0
#define BUZZER_PIN    7
#define NOKIA_DC      6
#define NOKIA_CS      4
#define NOKIA_RST     5
#define TEMP_AVG      2000
#define RATE_AVG      2000
#define CONTRAST_UP   A2
#define CONTRAST_DOWN A1


MAX6675 thermocouple(SCK, thermoCS, MISO);
FILTER1 FILTER1;
FILTER2 FILTER2;
Adafruit_PCD8544 display = Adafruit_PCD8544(SCK, MOSI, NOKIA_DC, NOKIA_CS, NOKIA_RST);
//Adafruit_PCD8544 display = Adafruit_PCD8544(NOKIA_DC, NOKIA_CS, NOKIA_RST);           //HARDWARE SPI
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!



float temperatureC = 20;
unsigned long previousMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
int thermoInterval = 211;//210;
unsigned long thermoMillis = 0;
float goalTemp = 0;
bool bakeStarted = false;
unsigned long startTime = 0;
unsigned long bakeMillis = 0;
unsigned long getGoalMillis = 0;
int stage = 0;
bool firstStageOne = true;
unsigned long prevMillis = 0;
float prevTemp = 0;
float tempRate = 0;
unsigned long stage1Ms = 0;
unsigned long stage2Ms = 0;
unsigned long stage3Ms = 0;
unsigned long stage4Ms = 0;
float adjustTempTo = 0;
int contrastSet = 55;
int xAxisSeconds = 336;//425;     //0 to 425 seconds
int yAxisTemperature = 288;//240; //0 to 240 degreesC
bool displayInfo = false;
bool needBuzz1 = true;
bool needBuzz2 = true;
unsigned long buzzMillis = 0;
int buzzDuration = 1000;



void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(CONTRAST_UP, INPUT_PULLUP);
  pinMode(CONTRAST_DOWN, INPUT_PULLUP);
  display.begin();
  display.setContrast(contrastSet); //Default 100 in display.begin();

  display.display(); // show splashscreen
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer
}

void loop() {

  if(analogRead(CONTRAST_UP)<500){
    contrastSet++;
    display.setContrast(contrastSet);
  }
  if(analogRead(CONTRAST_DOWN)<500){
    contrastSet--;
    display.setContrast(contrastSet);
  }

  //Serial.println(analogRead(buttonPin));
  if(analogRead(buttonPin)<500 && !bakeStarted){
    startTime = millis();
    bakeStarted = true;
    stage = 1;
    Serial.println("---STARTING BAKE CYCLE!---");
  }


  if(bakeStarted){
    bakeMillis = millis()-startTime;
    
    if(stage == 1){
      //PREHEAT:  temperatureC += 3 per 1000 ms;
      if(needBuzz1){
        buzzMillis = millis();
        needBuzz1 = false;
      }
      if(millis()-buzzMillis<=buzzDuration){
        digitalWrite(BUZZER_PIN, HIGH);
      }
      else{
        digitalWrite(BUZZER_PIN, LOW);
      }
      stage1Ms = bakeMillis;
      goalTemp = 150;
      adjustTempTo = 141;//100;
      if(temperatureC>=goalTemp || (stage1Ms>30000&&tempRate<0)){
        stage = 2;
      }
    }
  
    
    else if(stage == 2){
      //SOAK:
      stage2Ms = bakeMillis - stage1Ms;
      goalTemp = 150;
      adjustTempTo = 150;
      if(stage2Ms >= 100000){
        stage = 3;
      }
    }
    
    
    else if(stage == 3){
      //REFLOW:
      stage3Ms = bakeMillis - stage2Ms - stage1Ms;
      goalTemp = 215;//210;
      adjustTempTo = 215;//195;
      if(temperatureC>=goalTemp || (stage3Ms>30000&&tempRate<0)){
        stage = 4;
      }
    }
  
    
    else if(stage == 4){
      //COOLING:
      if(needBuzz2){
        buzzMillis = millis();
        needBuzz2 = false;
      }
      if(millis()-buzzMillis<=buzzDuration){
        digitalWrite(BUZZER_PIN, HIGH);
      }
      else{
        digitalWrite(BUZZER_PIN, LOW);
      }
      stage4Ms = bakeMillis - stage3Ms - stage2Ms - stage1Ms;
      goalTemp = 0;
      adjustTempTo = 0;
    }
  }

  if(millis()-thermoMillis>=thermoInterval){
    thermoMillis = millis();
  
    samplesThisSec++; //increment each time the loop cycles
    if(millis() - previousMillis >= 1000){ // Update value of samplesPerSec once every second:
      previousMillis=millis();
      samplesPerSec = samplesThisSec;
      samplesThisSec=0; 
      //Serial.println(samplesPerSec);  //print debug info
    }

    //SPI.begin();
    temperatureC = FILTER1.RUNNING_AVERAGE(
      thermocouple.readCelsius(), 
      samplesPerSec, 
      TEMP_AVG
    );
    if(millis()<3000){temperatureC=30;} //otherwise temperatureC == nan for some reason;
    //temperatureC = thermocouple.readCelsius();

    
    //tempRate = getTempRate(temperatureC, prevTemp, millis(), prevMillis);
    tempRate = FILTER2.RUNNING_AVERAGE(
      getTempRate(temperatureC, prevTemp, millis(), prevMillis),
      samplesPerSec,
      RATE_AVG
    );
    prevTemp = temperatureC;
    prevMillis = millis();
    
    Serial.print(temperatureC);
    Serial.print(" ");
    Serial.println(goalTemp);
  }


  
  if(temperatureC < adjustTempTo){
    digitalWrite(relayPin, HIGH);
  }
  else if(temperatureC > adjustTempTo){
    if(analogRead(buttonPin)<500){
      digitalWrite(relayPin, HIGH);   //override heating element;
      Serial.println("---OVERRIDING HEATING ELEMENT---");
    }
    else{
      digitalWrite(relayPin, LOW);
    }
  }

  


  //SPI.begin();
  display.clearDisplay();

  //TEMPERATURE:
  display.setTextSize(1);
  display.setTextColor(BLACK);


  if(displayInfo){
    display.setCursor(0,0);
    display.print(" STAGE: ");
    display.print(stage);
    display.print("  ");
    display.print(contrastSet);
    
    display.setCursor(0,8);
    if(stage != 4){
      display.print("  TIME: ");
      if(stage==0){display.print("0");}
      else if(stage==1){display.print(stage1Ms/1000);}
      else if(stage==2){display.print(stage2Ms/1000);}
      else if(stage==3){display.print(stage3Ms/1000);}
      //else if(stage==4){display.print(stage4Ms/1000);}
      display.print("s");
    }
    else if(stage==4){
      display.print("OPEN DOOR!");
    }
    
    display.setCursor(0,16);
    display.print("SET TO: ");
    display.print(adjustTempTo,1);
    display.print("C");
    
    display.setCursor(0,24);
    display.print("  GOAL: ");
    display.print(goalTemp,1);
    display.print("C");
    
    display.setCursor(0,32);
    display.print("ACTUAL: ");
    display.print(temperatureC,1);
    display.print("C");
  
    display.setCursor(0,40);
    display.print("  RATE: ");
    display.print(tempRate,1);
    display.print("C/s");
  }
  else{
    displayChart(xAxisSeconds, yAxisTemperature, bakeMillis/1000, temperatureC, stage);
  }
  display.display();

}




int chart[85] = {};
void displayChart(int chartWidth, int chartHeight, int bakeSeconds, float temperature, int _stage){
  
  //425*240x;
  int displayHeight = 48;
  int displayWidth = 84;
  float hDivider = (float)(chartWidth/displayWidth);
  float vDivider = (float)(chartHeight/displayHeight);
  
  if(bakeSeconds>chartWidth){
    bakeSeconds=chartWidth;
  }
  
  chart[(int)(bakeSeconds/hDivider)] = displayHeight-(int)(temperature/vDivider);
  
  display.setCursor(0,0);
  display.print(temperatureC,1);
  display.print("C");

  display.setCursor(42,0);
  display.print(tempRate,1);
  display.print("C/s");

  for(int i=0; i<(int)(bakeSeconds/hDivider); i++){
    //display.drawPixel(i,chart[i],BLACK);
    display.drawLine(i,chart[i], i+1,chart[i+1], BLACK);
  }

  if(_stage==0){
    display.setCursor(0,40);
    display.print(" PRESS START");
  }
  else if(_stage==1){
    display.setCursor(0,40);
    display.print("  1) RAMP-UP");
  }
  else if(_stage==2){
    display.setCursor(0,40);
    display.print("  2) SOAK");
  }
  else if(_stage==3){
    display.setCursor(0,40);
    display.print("  3) REFLOW");
  }
  else if(_stage==4){
    display.setCursor(0,40);
    display.print("  4)OPEN DOOR!");
  }
  
}





float getTempRate(float currentTemp, float previousTemp, unsigned long currentMs, unsigned long previousMs){
  return (currentTemp-previousTemp)*1000/(float)(currentMs-previousMs);
}






