#include "MS5611.h"
#include "MS5611_I2C.h"
#include "FILTER.h"
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "BEEP.h"
#include "DEFAULT_SETTINGS.h"
#include "BUTTON.h"


#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define POWER_PIN                     A0    // DEVICE POWERS OFF IF THIS PIN GOES LOW;
#define MS5611_CSB                    13    // Chip/Slave Select Pin
#define D1_OSR                         5    // (Default pressure OSR mode 5) 
#define D2_OSR                         2    // (Default temperature OSR mode 2) 
/*#define EPD_CS                         6
#define EPD_DC                        10
#define EPD_RST                       11
#define EPD_BUSY                      12*/
#define PRESSURE_FILTER_DURATION       0    // (AVERAGING DURATION: 1ms to 2000ms)
#define ALTITUDE_FILTER_DURATION    1000    // (AVERAGING DURATION: 1ms to 2000ms)
#define VSPEED_FILTER_DURATION      1000    // (AVERAGING DURATION: 1ms to 2000ms)
#define TEMPERATURE_FILTER_DURATION    0    // (AVERAGING DURATION: 1ms to 2000ms)
#define POT_CS                        A3
#define BEEP_PIN                      A5    // (Default A5) Pin connected to buzzer 
#define BUTTON_PIN                    A1    // CONNECT THE OTHER BUTTON LEAD TO 3.3V;


MS5611_SPI MS5611;
MS5611_I2C MS5611_I2C; 
FILTER1 FILTER1;
FILTER2 FILTER2;
FILTER3 FILTER3;
FILTER4 FILTER4;
BEEP BEEP;
Default SETTING;
BUTTON BUTTON;

#define COLORED     0
#define UNCOLORED   1
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;

float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
unsigned long previousMillis = 0;
unsigned long displayMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second

void adjustVolumeTo(int volLevel);
unsigned long loops = 0;


void setup() {  
  
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);  //ONCE THIS PIN GOES LOW, THE DEVICE POWERS OFF; 
  
  Serial.begin(BAUD_RATE);
  //while(!Serial);
  
  //MS5611.begin(MS5611_CSB);
  MS5611_I2C.begin();
  MS5611_I2C.setOversampling(MS5611_HIGH_RES);

  /*BEEP.begin(BEEP_PIN);
  BEEP.setClimbThreshold(SETTING.CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SETTING.SINK_ALARM_TRIGGER);   //ft/s
  BEEP.setClimbPitchMax(SETTING.CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(SETTING.CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SETTING.SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SETTING.SINK_PITCH_MIN);             //Hz*/  
  //for(int i=0; i<10; i++){
  //SETTING.VOLUME++;
  //if(SETTING.VOLUME>4){SETTING.VOLUME=0;}
  
  //adjustVolumeTo(SETTING.VOLUMES[SETTING.VOLUME]);
  //tone(BEEP_PIN, 400, 250);
  
  //delay(250);
  //}

  //epd.begin(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY);
      //INITIALIZE FULL UPDATE MODE AT START UP TO COMPLETELY ERASE THE SCREEN: 
      if (epd.Init(lut_full_update) != 0) { Serial.println("e-Paper init failed"); }
      else{Serial.println("e-Paper init full success"); }
      //GET RID OF ANY GHOST IMAGE THAT REMAINS BY FLICKERING THE ENTIRE SCREEN:
      epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
      epd.DisplayFrame();
      epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
      epd.DisplayFrame();
      //INITIALIZE PARTIAL UPDATE MODE AFTER GHOST IMAGES ARE CLEARED:
      if (epd.Init(lut_partial_update) != 0) { Serial.println("e-Paper init failed"); }
      else{Serial.println("e-Paper init partial success"); }
      //???GET RID OF ANY GHOST IMAGE THAT REMAINS BY FLICKERING THE ENTIRE SCREEN:
      epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
      epd.DisplayFrame();
      epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
      epd.DisplayFrame();
}

void loop() {
  samplesThisSec++; //increment each time the loop cycles
  if(millis() - previousMillis >= 1000){ // Update value of samplesPerSec once every second:
    previousMillis=millis();
    samplesPerSec = samplesThisSec;
    samplesThisSec=0; 
    //Serial.println(samplesPerSec);  //print debug info
  }

      //====MS5611=================================================================/
        
        //TEMPERATURE:
        if(TEMPERATURE_FILTER_DURATION){
          temperatureF = FILTER4.RUNNING_AVERAGE(
            //MS5611.getTemperatureF(D2_OSR), 
            MS5611_I2C.readTemperature(),
            samplesPerSec, 
            TEMPERATURE_FILTER_DURATION
          );
          //temperatureF = 77;
        }
        else{
          //temperatureF = MS5611.getTemperatureF(D2_OSR);
          temperatureF = MS5611_I2C.readTemperature();
        }
        
        //PRESSURE:
        if(PRESSURE_FILTER_DURATION){
          pressurePa = FILTER1.RUNNING_AVERAGE(
            //MS5611.getPressurePa(D1_OSR),
            MS5611_I2C.readPressure(), 
            samplesPerSec, 
            PRESSURE_FILTER_DURATION
          );
        }
        else{
          //pressurePa = MS5611.getPressurePa(D1_OSR);
          pressurePa = MS5611_I2C.readPressure();
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

        //BEEP:
        //adjustVolumeTo(SETTING.VOLUMES[SETTING.VOLUME]);
        if(SETTING.ENABLE_BEEP && millis() > 7000){                                                                //BEEP
          //if(SETTING.BEEP_TYPE == 1){BEEP.basedOnVelocity(altitudeFt, velocityFtPerSec, millis());}
          //if(SETTING.BEEP_TYPE == 2){BEEP.bufferedDurationIncrements(altitudeFt, velocityFtPerSec, millis());}
        }
        

        //DEBUG:
        //Serial.println(temperatureF); 
        //Serial.println(pressurePa);
        //Serial.println(altitudeFt); 
        //Serial.print(" "); 
        Serial.println(velocityFtPerSec);



      //====EPD====================================================================/
        //WAIT FOR A CERTAIN NUMBER OF LOOPS BEFORE UPDATING THE EPAPER DISPLAY:
        if(loops++%/*1305*/900==0){
          if(!SETTING.ENABLE_OLED){SETTING.ENABLE_OLED=true; Serial.println("EPD ON");}
          else{SETTING.ENABLE_OLED=false; epd.Reset(); Serial.println("EPD OFF");}
        }
        if(SETTING.ENABLE_OLED && /*false &&*/ /*millis()-displayMillis>=1000 &&*/ digitalRead(12)==LOW){
          //delay(10);
          displayMillis = millis();
  /*
   *  
   *   |<------112px------>|
   *   |    |<----86px---->|
   *  _|____|______________|________
   * |_______TOP_BAR_______|______|_----- 18px
   * |     /  \            |      ^
   * |   /       \         |      |
   * |  / VARIO     \      |      |
   * | |              _\___|      |
   * | |_____________|     |      |
   * | |        __,--|_____|     158px
   * | | __,---'           |      |
   * |  \                  |      |
   * |   \                 |      |
   * |_____\_______________|______v_
   * |       ALTITUDE      |      ^_____28px
   * |_____________________|______v_
   * |                     |      ^
   * |       CHART         |      |
   * |                     |     92px
   * |                     |      |
   * |                     |      |
   * |_____________________|______v_
   */          
          // ______________________
          //|_______TOP_BAR:_______|
          //PRINT CLOCK:
          unsigned long time_now_s = (millis()) / 1000;
          char time_string[] = {'0', '0', ':', '0', '0', ':', '0', '0', '\0'};
          // 12:34:56 == 45296_s
          time_string[0] = time_now_s / 60 / 60 / 10 + '0'; //ten hr  ---> 45296 / 60 / 60 / 10 == 1
          time_string[1] = time_now_s / 60 / 60 / 10 + '0'; //one hr  ---> 45296 / 60 / 60 % 10 == 2
          time_string[3] = time_now_s / 60 / 10 + '0';      //ten min ---> 45296 / 60 % 60 / 10 == 3
          time_string[4] = time_now_s / 60 % 10 + '0';      //one min ---> 45296 / 60 % 10      == 4
          time_string[6] = time_now_s % 60 / 10 + '0';      //ten sec ---> 45296 % 60 / 10      == 5
          time_string[7] = time_now_s % 60 % 10 + '0';      //one sec ---> 45296 % 60 % 10      == 6
          paint.SetRotate(ROTATE_180);
          paint.SetWidth(128);
          paint.SetHeight(18);
          paint.Clear(UNCOLORED);
          paint.DrawStringAt(1, 1, time_string, &Font16, COLORED);  
          //TODO -- VOLUME ICON:
          //if(displayLoop%2 && SETTING.ENABLE_BEEP){ for(int j=2; j<=12; j++){ for(int i=0; i<=SETTING.VOLUME; i++){ paint.DrawPixel(110+i,j,COLORED); }}}
          paint.DrawHorizontalLine(0, 17, 128, COLORED);
          epd.SetFrameMemory(paint.GetImage(), 0, 278, paint.GetWidth(), paint.GetHeight());
          
          // _____________________
          //|________VARIO________|
          //PRINT VELOCITY:
          char velocity_string[] = {'-', '0', '0', ' '};
          // -9_ft/s == {' ', '-', '9'};
          if(velocityFtPerSec<=-10){      velocity_string[0] = '-';    velocity_string[1] = abs(velocityFtPerSec/10)+'0';    velocity_string[2] = abs((int)velocityFtPerSec%10)+'0'; }
          else if(velocityFtPerSec<0){    velocity_string[0] = ' ';    velocity_string[1] = '-';                             velocity_string[2] = abs((int)velocityFtPerSec)+'0';    }
          else if(velocityFtPerSec<10){   velocity_string[0] = ' ';    velocity_string[1] = ' ';                             velocity_string[2] = (int)velocityFtPerSec+'0';         }
          else{                           velocity_string[0] = ' ';    velocity_string[1] = (velocityFtPerSec/10)+'0';       velocity_string[2] = ((int)velocityFtPerSec%10)+'0';    }
          paint.SetRotate(ROTATE_180);
          paint.SetWidth(51);
          paint.SetHeight(28);
          paint.Clear(UNCOLORED);
          paint.DrawRectangle(1, 1, 55, 27, COLORED);
          paint.DrawStringAt(1, 4, velocity_string, &Font24, COLORED);
          //paint.DrawHorizontalLine(0, 17, 128, COLORED);
          epd.SetFrameMemory(paint.GetImage(), 0, 170, paint.GetWidth(), paint.GetHeight());
          
          //TODO -- NEEDLE:
          paint.SetRotate(ROTATE_180);
          paint.SetWidth(64);
          paint.SetHeight(120);
          paint.Clear(UNCOLORED);
          //paint.DrawStringAt(1, 1, time_string, &Font16, COLORED);
          paint.DrawHorizontalLine(0,60,64,COLORED);
          paint.DrawLine(0, 59-5*velocityFtPerSec, 64, 59, COLORED);
          paint.DrawLine(0, 60-5*velocityFtPerSec, 64, 60, COLORED);
          paint.DrawLine(0, 61-5*velocityFtPerSec, 64, 61, COLORED);
          epd.SetFrameMemory(paint.GetImage(), 63, 124, paint.GetWidth(), paint.GetHeight());
          
          // _____________________
          //|_______ALTITUDE______|
          //PRINT ALTITUDE:
          //UP TO FIVE DIGITS, OR UP TO FOUR DIGITS WITH A NEGATIVE SIGN:
        
          //TEMPORARILY REMOVE THE NEGATIVE SIGN BEFORE CONVERSION TO ASCII:
          int absNumber = abs(altitudeFt);
          
          //PARSE EACH DIGIT AND CONVERT TO ASCII CHARACTERS:
          int digit[] = {0,0,0,0,0};
          digit[4] = ((absNumber/10000)); //________________________________________________________________________________________________ = 0
          digit[3] = ((absNumber/1000) - (digit[4]*10)); //_________________________________________________________________________ = 4 - 0 = 4
          digit[2] = ((absNumber/100) - (digit[4]*100) - (digit[3]*10)); //___________________________________________________ = 45 - 0 - 40 = 5
          digit[1] = ((absNumber/10) - (digit[4]*1000) - (digit[3]*100) - (digit[2]*10)); //___________________________ = 451 - 0 - 400 - 50 = 1
          digit[0] = ((absNumber) - (digit[4]*10000) - (digit[3]*1000) - (digit[2]*100) - (digit[1]*10)); //___ = 4513 - 0 - 4000 - 500 - 10 = 3
          //EXAMPLE: digit[]={3,1,5,4,0} == ALTITUDE OF 4513;
        
          //SHIFT THE VALUES OF EACH DIGIT TO MATCH WITH ASCII TABLE: (' ' == 0 , '0' == 48 , '2' == 50 , ...)
          for(int i=0; i<=4; i++){digit[i]+=48;}
        
          //RE-INSERT NEGATIVE SIGN IF NEEDED AND TRIM LEADING ZEROS:
          if(altitudeFt<0){
            if(digit[1]=='0' && digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
              digit[1]='-'; 
              for(int i=2; i<=4; i++){digit[i]=' ';}
            }
            else if(digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
              digit[2]='-';
              for(int i=3; i<=4; i++){digit[i]=' ';}
            }
            else if(digit[3]=='0' && digit[4]=='0'){
              digit[3]='-';
              digit[4]=' ';
            }
            else{
              digit[4]='-';
            }
          }
        
          //FOR POSITIVE NUMBERS, SIMPLY REMOVE LEADING ZEROS
          else{
            if(digit[1]=='0' && digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
              for(int i=1; i<=4; i++){digit[i]=' ';}
            }
            else if(digit[2]=='0' && digit[3]=='0' && digit[4]=='0'){
              for(int i=2; i<=4; i++){digit[i]=' ';}
            }
            else if(digit[3]=='0' && digit[4]=='0'){
              for(int i=3; i<=4; i++){digit[i]=' ';}
            }
            else if(digit[4]=='0'){
              digit[4]=' ';
            }    
          }
          
          //DEBUG:
          //while(!Serial){};
          //for(int i=4; i>=0; i--){delay(10); Serial.print(digit[i]);} Serial.println();  
          
          paint.SetRotate(ROTATE_180);
          paint.SetWidth(128);
          paint.SetHeight(28);
          paint.Clear(UNCOLORED);
          //BOUNDING BOX:
          paint.DrawRectangle(1, 1, 88, 27, COLORED);
          for(int i=4; i>=0; i--){int j = 4-i; paint.DrawCharAt(2+(17*j), 4, digit[i], &Font24, COLORED);}
          epd.SetFrameMemory(paint.GetImage(), 0, 92, paint.GetWidth(), paint.GetHeight());
          epd.DisplayFrame();
        }
        else if(!SETTING.ENABLE_OLED){
          //delayMicroseconds(192);
          delay(2);
        }/*end epd*/



    /*if(millis()>10000){BUTTON.PRESS=BUTTON.CHECK(BUTTON_PIN, millis());}
    if(BUTTON.PRESS==BUTTON.CLICK){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      
      //SETTING.VOLUME++;
      //if(SETTING.VOLUME>4){SETTING.VOLUME=0; SETTING.ENABLE_BEEP=0;}
      //else if(SETTING.VOLUME==1){SETTING.ENABLE_BEEP=1;}
      //adjustVolumeTo(SETTING.VOLUMES[SETTING.VOLUME]);
      //Serial.print("adjustVolumeTo("); 
      //Serial.print(SETTING.VOLUMES[SETTING.VOLUME]); 
      //Serial.println(")");
      
      SETTING.ENABLE_BEEP=!SETTING.ENABLE_BEEP;
      if(SETTING.ENABLE_BEEP){SETTING.ENABLE_OLED=false; epd.Reset();}
      else{SETTING.ENABLE_OLED=true;}
      if(SETTING.ENABLE_BEEP){tone(BEEP_PIN, 400, 250);}
    }
    if(BUTTON.PRESS==BUTTON.HOLD){
      BUTTON.PRESS=BUTTON.NO_ACTION; 
      SETTING.ENABLE_OLED=!SETTING.ENABLE_OLED;
      digitalWrite(POWER_PIN, LOW); //POWER OFF;
    }*/     
       

}/*end loop*/






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
