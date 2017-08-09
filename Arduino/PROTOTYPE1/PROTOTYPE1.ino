/* LIBRARIES FOR BMP180 TEMPERATURE AND PRESSURE SENSOR */
#include <Wire.h>
#include <Adafruit_BMP085.h>
/********************************************************/

/* LIBRARIES FOR NOKIA 5110 LCD DISPLAY */
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
/****************************************/

float climbThreshold = 1;
float sinkAlarm = -6;

const int buzzerPin = A2;                  // Buzzer Pin
float buzzerPitch = 400;
float buzzInterval = 1000;
unsigned long buzzOnMillis = 1000;
unsigned long buzzOffMillis = 1000;
  float beepsPerSec;
  float pitch;
  

Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);
Adafruit_BMP085 sensor;

/* TEMPERATURE */
  float tempC;
  float tempF;                                      // Resembles the temperature(degreesF) at the beginning of each checkInterval
  float temperature2;             // Resembles the temperature(degreesF) at the end of each checkInterval, also the current temperature(degreesF)
/***************/

/* PRESSURE */  
  float pressurePa;                                 // Resembles the pressure(Pa) at the beginning of each checkInterval
  const float pressureSeaLevel = 101325.00;         // (Pa)
  float pressure2;                // Resembles the pressure(Pa) at the end of each checkInterval, also the current pressure(Pa)          
/************/

/* FOR CALIBRATING ALTITUDE */
  // The V = 6341 - 4600;
  // North Side Bench = 5100 - 4800;
  // North Side Top = ???;
  // South Side = 5150 - 4850;
  // Ben Lomond = 8400 - 4883;
  
  float onGround = 0;
  float aboveGround;
/****************************/ 

/* ALTITUDE */
  float altitude1 = onGround;             // The first of three consecutive, most recent values of altitude calculations
  float altitude2 = onGround;             // Resembles the previous altitude(ft) calculation
  float altitude3 = onGround;             // Resembles the currently calculated altitude(ft)                     
  float altiChangeLimit = 5;              // Altitude is limited to change (+/-) this value each time it is filtered with filterLC();
/************/

/* VELOCITY */
  float velocity1 = 0;                // The first of three consecutive, most recent values of velocity calculations
  float velocity2 = 0;                // Resembles the previous velocity(ft/s) calculation
  float velocity3 = 0;                // Resembles the currently calculated velocity(ft/s)
  float veloChangeLimit = 0.5;        // Velocity is limited to change (+/-) this value each time it is filtered with filterLC();
/************/

/* FOR TESTING VARIOUS FILTERS */
  float A_RAW;        // RAW Altitude Calibrated to Zero
  float A_LC;         // Altitude: LimitedChangeFilter
  float A_LS;         // Altitude: LinearSmoothingFilter
  float A_LC_LS;      // Altitude: LimitedChangeFilter & LinearSmoothingFilter (in this order)
  float A_LS_LC;      // Altitude: LinearSmoothingFilter & LimitedChangeFilter (in this order)
  float A_LC_3AV;     // Altitude: Three most recent values averaged after passing through limitedChange Filter
  
  float V_RAW;        // RAW Velocity
  float V_LC;         // Velocity: LimitedChangeFilter
  float V_LS;         // Velocity: LinearSmoothingFilter
  float V_LC_LS;      // Velocity: LimitedChangeFilter & LinearSmoothingFilter (in this order)
  float V_LS_LC;      // Velocity: LinearSmoothingFilter & LimitedChangeFilter (in this order)

/* 
 * TEST CASES: (25)
 *  A_RAW & V_RAW
 *  ...
 *  ...
 *  A_LS_LC & A_LS_LC
*/
  
/*******************************/

/* TIME INTERVAL TO GATHER SENSOR DATA */
  unsigned long checkInterval = 100;            // check the sensor readings every ___ (milliseconds)
  unsigned long previousMillis = 500;            // Start of each time interval to check sensor readings (milliseconds)
  unsigned long currentMillis;
/***************************************/

/* TO CALCULATE ALTITUDE AND VELOCITY */
  float getAltitude(float S, float P, float T);      // S == pressureSeaLevel;  P == pressure;  T == temperature;
  float getVelocity(float A1, float A2, unsigned long I);   // A1 == altitude1;  A2 == altitude2;  I == checkInterval;
  float dejaVu(float V);
/**************************************/

/* FILTERS */
  void filterLS(float value1, float value2, float value3);  // change to float function... if problems persist.
  float filterLC(float value1, float value2, float changeLimit);
/***********/

/* LCD CURSOR COORDINATES */
  int cx;
  int cy;
/**************************/ 

/* TO DISPLAY UNIQUE CHARACTERS ON LCD */
  void arrowUp(int cx, int cy);
  void arrowDown(int cx, int cy);
  void degree(int cx, int cy);
/***************************************/

//////////////////////////////////////////////////////////////// SETUP ///////////////////////////////////////////////////////////

void setup()   {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  sensor.begin();
  display.begin();
  display.setContrast(50);
  display.setTextColor(BLACK);
  display.clearDisplay();       // clears the screen and buffer

/* TEST BUZZER */
  tone(buzzerPin, buzzerPitch);
  delay(250);
  noTone(buzzerPin);
/***************/

/* TEST LCD DISPLAY *  
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Is it");
  display.display();          // show splashscreen
  delay(250);
  display.clearDisplay();     // clears the screen and buffer
  display.display();          // show splashscreen
  delay(50);                  // wait 
  display.println("Working?");
  display.display();
  delay(250);                 // wait
  display.clearDisplay();     // clears the screen and buffer
  display.display();          // show splashscreen
/********************/
 
/* CALIBRATE ALTITUDE */
  pressurePa = sensor.readPressure();                                   // Read current pressure(Pa)
  tempF = (sensor.readTemperature()) * 1.8 + 32;                        // Read current temperature(degreesF)
  altitude3 = getAltitude(pressureSeaLevel, pressurePa, tempF);         // Calculate current altitude(ft)
  Serial.print("PRE-CALIBRATED ALTITUDE: ");
  Serial.println(altitude3);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("PRE-CALIBRATED ALTITUDE: ");
  display.println("");
  display.print(altitude3);
  display.println(" ft");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
  
  // Figure out how far off the altitude accuracy is, known as: "aboveGround" 
  // (because usually the reading is too high)... I'll work on that later.
  aboveGround = altitude3 - onGround;         
  
  // Start all the variables at the same place, 
  // so the filters are already all caught up.
  altitude3 = onGround;
  display.println("...NOW,");
  display.println("CALIBRATED TO:");
  display.println("");
  display.print(altitude3);
  display.println(" ft");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();

  /* POM SOUTH SIDE *
  display.println("  Point of"); 
  display.println("the Mountain,");
  display.println(" South Side");
  display.println("   Launch");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
/****************************/

  /* ZERO = 0 *
  display.println("           "); 
  display.println("    ZERO   ");
  display.println("  ALTITUDE  ");
  display.println("           ");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
/***************************/

  /* HOME = 4700 */
  display.println("           "); 
  display.println("    HOME   ");
  display.println("    LOGAN  ");
  display.println("           ");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
/***************************/

  /* THE V *
  display.println("           "); 
  display.println("   The V   ");
  display.println("   Launch  ");
  display.println("           ");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
/****************************/
                         
  altitude2 = altitude3;
  altitude1 = altitude3;
  A_LC = altitude3;
  A_LC_3AV = altitude3;
/**********************/  

/* SERIAL TEST DATA HEADERS */
Serial.println("DRIVING IN THE CAR, FAST AS I CAN, UP AND DOWN HILLS, WINDOWS DOWN, FOAM COVERING PRESSURE SENSOR");
Serial.print("Time(s)");
Serial.print("  ");
Serial.print("A_RAW");
Serial.print("  ");
Serial.print("V_RAW");
Serial.print("  ");
Serial.print("A_LC_3AV (LIMITED TO 5.0)");
Serial.print("  ");
Serial.println("V_LC (LIMITED TO 0.5)");  

/****************************/
    
}

///////////////////////////////////////////////////////////// LOOP /////////////////////////////////////////////////////////////////

void loop() {
   
/***************************************************** (1) CALCULATIONS ***********************************************************/
    
    currentMillis = millis();      // Projected to be the conclusion of each checkInterval(milliseconds) to check sensor readings 
    if(currentMillis - previousMillis >= checkInterval) {                 // If it's time to check the sensor readings again:
    previousMillis = currentMillis;

    pressurePa = sensor.readPressure();               // Read current pressure in (Pa)
    tempF = (sensor.readTemperature()) * 1.8 + 32;    // Read current temperature and convert to (degreesF)



// KEEP TRACK OF THE PREVIOUS TWO CALCULATED ALTITUDE VALUES
  altitude1 = altitude2;            
  altitude2 = A_LC_3AV;            
  
// CALCULATE ALTITUDE
  altitude3 = getAltitude(pressureSeaLevel, pressurePa, tempF) - aboveGround; /*- onGround;*/
  A_RAW = altitude3;

// LIMITED CHANGE FILTER "LC"
  A_LC = filterLC(altitude2, A_RAW, altiChangeLimit);

// AVERAGE THE LAST THREE ALTITUDE VALUES
  A_LC_3AV = (A_LC + altitude2 + altitude1)/3.;

/* LINEAR SMOOTHING FILTER "LS"
  filterLS(altitude1, altitude2, altitude3);  
  A_LC_LS = altitude3; */



// KEEP TRACK OF THE PREVIOUS TWO CALCULATED VELOCITY VALUES  
  velocity1 = velocity2;
  velocity2 = V_LC;   

// CALCULATE VELOCITY  
  velocity3 = getVelocity(altitude2, A_LC_3AV, checkInterval);
  V_RAW = velocity3;

// LIMITED CHANGE FILTER "LC"
  V_LC = filterLC(velocity2, V_RAW, veloChangeLimit); 

/* LINEAR SMOOTHING FILTER "LS"
  filterLS(velocity1, velocity2, velocity3);
  V_LC_LS = velocity3; */  
 
/********************************************************** (2) SERIAL ************************************************************/
/*
   Serial.print("velocity3: ");
   Serial.print(velocity3);
      Serial.print(" v2: ");
      Serial.print(velocity2); 
        Serial.print(" v1: ");
        Serial.println(velocity1);
   
   Serial.print("altitude3: ");
   Serial.print(altitude3);
      Serial.print(" a2: ");
      Serial.print(altitude2); 
        Serial.print(" a1: ");
        Serial.println(altitude1);
          Serial.println("");
*/

Serial.print(millis()/1000.);
Serial.print("  ");
Serial.print(A_RAW);
Serial.print("  ");
Serial.print(V_RAW);
Serial.print("  ");
Serial.print(A_LC_3AV);
Serial.print("  ");
Serial.println(V_LC); 
    
/************************************************************ (3) DISPLAY *******************************************************/
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(BLACK);
   display.setCursor(0,0);

   // DISPLAY TEMPERATURE
   display.print(tempF,0);
   degree(12,0);
   display.print(" F"); 

   // DISPLAY TIME
   display.print("  ");
   display.print(millis()/1000/60.,1);
   display.print("min");

/* // DISPLAY PRESSURE
   display.setCursor(0,11);
   display.print("P: ");    
   display.setCursor(15,11);
   display.print(pressurePa,0);
   display.print(" Pa"); 
*/
   // DISPLAY ALTITUDE
   display.setTextSize(2);    
   display.setCursor(15,12);
   display.print(A_LC_3AV,0);
   display.setTextSize(1);
   display.print("ft"); 

   // DISPLAY VERTICAL VELOCITY
   display.setCursor(0,32);   
   if(V_LC<0){arrowDown(15,32);} else if(V_LC>=0){arrowUp(15,32);}
   display.setCursor(22,32);
   display.print(abs(V_LC),1);
   display.print(" ft/s"); 
   
   display.display();         // show splashscreen
   
   } /*end of all operations at each checkInterval*/
   

/********************************************************** (4) BUZZER ***********************************************************/

// BEEP CONDITIONS            
  if(V_LC > climbThreshold && V_LC < 10) {buzzInterval = 1000 / ((V_LC/2.) + 2);}            
  else if(V_LC >= 10) {buzzInterval = 1000 / 7.;}                            // 7 bps == 1000 millis / 7 cycles of beep (on/off)            
  else if(V_LC < sinkAlarm) {                                                       // Hold the beep constantly ON
    buzzInterval = 1000; 
    buzzOnMillis = buzzInterval; 
    buzzOffMillis = currentMillis;
    }                
  else if(V_LC >= sinkAlarm && V_LC <= climbThreshold) {                                         // Turn the buzzer off  
    buzzInterval = 1000;
    buzzOnMillis = currentMillis;
    buzzOffMillis = buzzInterval;
    }                        
  else {Serial.println("ERROR: BEEP DOESN'T KNOW WHAT TO DO!");}

// PITCH CONDITIONS           
  if(V_LC > climbThreshold && V_LC < 10) {buzzerPitch = (V_LC*42) + 380;}            
  else if(V_LC >= 10) {buzzerPitch = 800;}            
  else if(V_LC < sinkAlarm) {buzzerPitch = 250;}           
  else if(V_LC >= sinkAlarm && V_LC <= climbThreshold) {buzzerPitch = 0;}
  else {Serial.println("ERROR: PITCH DOESN'T KNOW WHAT TO DO!");}


// If it's time to turn the buzzer on...
if(currentMillis - buzzOnMillis > buzzInterval/2.){

  buzzOnMillis = currentMillis;
    
/* TURN ON BUZZER */
  tone(buzzerPin, buzzerPitch);
/***************/
 
}
  
// If it's time to turn the buzzer off...
if(currentMillis - buzzOffMillis > buzzInterval){

  buzzOffMillis = currentMillis;
  
/* TURN OFF BUZZER */
  
  noTone(buzzerPin);
/***************/

}

}  /*end of loop*/



/////////////////////////////////////////////////////////// FUNCTION DEFINITIONS ///////////////////////////////////////////////////

float getAltitude(float S, float P, float T){
  // return 3.28084*((((S*100)/(P*100))^(1/5.257)-1)*((T-32)*(5/9)+273.15))/0.0065;
  return 504.745*((5*(T - 32))/9 + 273.15)*(pow((S/P),0.190223) - 1);
}

float getVelocity(float A1, float A2, unsigned long I){
  float altitudeChange = A2 - A1;
  float timeConversion = 1000./I;
  float V = timeConversion * altitudeChange;
  return V;  
}

float dejaVu(float V){
  static float a = 0;
  float b = a;
  a = V;
  return b;
}

void arrowUp(int cx, int cy){   
// draw a single pixel at a time to form an up arrow:
/* 
 * _|0_1_2_3_4_5|
 * 0|O-O-X-O-O-O|
 * 1|O-X-X-X-O-O|
 * 2|X-X-X-X-X-O|
 * 3|O-O-X-O-O-O|
 * 4|O-O-X-O-O-O|
 * 5|O-O-X-O-O-O|
 * 6|O-O-X-O-O-O|
*/
  display.drawPixel(cx+2, cy+0, BLACK); 
  display.drawPixel(cx+1, cy+1, BLACK);
  display.drawPixel(cx+2, cy+1, BLACK);
  display.drawPixel(cx+3, cy+1, BLACK); 
  display.drawPixel(cx+0, cy+2, BLACK); 
  display.drawPixel(cx+1, cy+2, BLACK);
  display.drawPixel(cx+2, cy+2, BLACK);
  display.drawPixel(cx+3, cy+2, BLACK);
  display.drawPixel(cx+4, cy+2, BLACK);
  display.drawPixel(cx+2, cy+3, BLACK); 
  display.drawPixel(cx+2, cy+4, BLACK);
  display.drawPixel(cx+2, cy+5, BLACK); 
  display.drawPixel(cx+2, cy+6, BLACK); 
}

void arrowDown(int cx, int cy){   
// draw a single pixel at a time to form a down arrow: 
/* 
 * _|0_1_2_3_4_5|
 * 0|O-O-X-O-O-O|
 * 1|O-O-X-O-O-O|
 * 2|O-O-X-O-O-O|
 * 3|O-O-X-O-O-O|
 * 4|X-X-X-X-X-O|
 * 5|O-X-X-X-O-O|
 * 6|O-O-X-O-O-O|
*/
  display.drawPixel(cx+2, cy+0, BLACK); 
  display.drawPixel(cx+2, cy+1, BLACK); 
  display.drawPixel(cx+2, cy+2, BLACK); 
  display.drawPixel(cx+2, cy+3, BLACK); 
  display.drawPixel(cx+0, cy+4, BLACK);
  display.drawPixel(cx+1, cy+4, BLACK); 
  display.drawPixel(cx+2, cy+4, BLACK); 
  display.drawPixel(cx+3, cy+4, BLACK); 
  display.drawPixel(cx+4, cy+4, BLACK);  
  display.drawPixel(cx+1, cy+5, BLACK); 
  display.drawPixel(cx+2, cy+5, BLACK);
  display.drawPixel(cx+3, cy+5, BLACK);
  display.drawPixel(cx+2, cy+6, BLACK); 
}

void degree(int cx, int cy){   
// draw a single pixel at a time to form a degrees symbol:
/* 
 * _|0_1_2_3_4_5|
 * 0|O-O-O-X-O-O|
 * 1|O-O-X-O-X-O|
 * 2|O-O-O-X-O-O|
 * 3|O-O-O-O-O-O|
 * 4|O-O-O-O-O-O|
 * 5|O-O-O-O-O-O|
 * 6|O-O-O-O-O-O|
*/
  display.drawPixel(cx+3, cy+0, BLACK); 
  display.drawPixel(cx+2, cy+1, BLACK); 
  display.drawPixel(cx+4, cy+1, BLACK); 
  display.drawPixel(cx+3, cy+2, BLACK); 
}

void filterLS(float value1, float value2, float value3){
  // LINEAR SMOOTHING FILTER "LS"
  /* 
   *  This filter needs to know the THREE most recent data values.
   *  Three consecutive values must be either all increasing or all decreasing,
   *  else the current value is rolled back to the previous value.
   *  value3 resembles the current value, or most recent value.
   *  value2 resembles the previous value, or the second value in the series.
   *  value1 resembles the first value in the series.
  */
  /*if(value3 <= value2 && value2 <= value1){value3 = value3;}*/  // This line is redundant
  if(value3 <= value2 && value2 >= value1){value3 = value2;}
  /*else if(value3 >= value2 && value2 >= value1){value3 = value3;}*/  // This line is redundant
  else if(value3 >= value2 && value2 <= value1){value3 = value2;}
}

      
float filterLC(float value1, float value2, float changeLimit){
  // LIMITED CHANGE FILTER "LC"
  /*
   * This filter needs to know the TWO most recent data values.
   * Limit how much the value is allowed to change each time a new value is calculated. 
   * value2 resembles the current value, or most recent value.
   * value1 resembles the previous value, or the first value in the series.
  */
  float LC;
  if(value2 > value1 + changeLimit){LC = value1 + changeLimit;}
  else if(value2 < value1 - changeLimit){LC = value1 - changeLimit;}
  else {LC = value2;}
  return LC;
}
