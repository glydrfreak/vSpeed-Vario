#include "MS5611.h"
#include "BEEP.h"
#include "FILTER.h"
#include "vAdafruit_BluefruitLE_SPI.h"
#include "DEFAULT_SETTINGS.h"
//#include "X9C.h"

#define POT_UD                        A0
#define POT_INC                       A1
#define VOL_DOWN                      A2
#define VOL_UP                        A3
#define POT_CS                        A4
#define BEEP_CTRL                     A5
#define MS5611_CSB                    13
#define VBATPIN                        9    // Pin monitors battery level (Pin A7)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended set 4, set to -1 if unused...


#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define D1_OSR                         5    // (Default pressure OSR mode 5) 
#define D2_OSR                         2    // (Default temperature OSR mode 2) 
#define VERBOSE_MODE               true    // If set to 'true' enables debug output
#define PRESSURE_FILTER_DURATION       0    // (AVERAGING DURATION: 1ms to 2000ms)
#define ALTITUDE_FILTER_DURATION    1000    // (AVERAGING DURATION: 1ms to 2000ms)
#define VSPEED_FILTER_DURATION       750    // (AVERAGING DURATION: 1ms to 2000ms)
#define TEMPERATURE_FILTER_DURATION    0    // (AVERAGING DURATION: 1ms to 2000ms)


Default SETTING;
MS5611_SPI MS5611;
FILTER1 FILTER1;
FILTER2 FILTER2;
FILTER3 FILTER3;
FILTER4 FILTER4;
BEEP BEEP;
vAdafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
//X9C X9C;



void SWITCH_BLE_MODE(int bluetoothMode);    // BLE MODULE INITIALIZES ACCORDING TO SPECIFIED BLUETOOTH_MODE;
void receiveCommands();
void transmitVspeed();
void transmitFlySkyHy();
void adjustVolumeTo(int volLevel);  // 0 - 100;
void volumeUp();
void volumeDown();
float getBatteryLvl();    //Volts



float temperatureF = 0;
float pressurePa = 0;
float altitudeFt = 0;
float velocityFtPerSec = 0;
unsigned long previousMillis = 0;
int samplesThisSec = 0;    // Used for calculating averaging duration
int samplesPerSec = 0;     // Used for displaying samplesPerSec updated every once second
int BLUETOOTH_MODE = 0;    // iOS_Flyskyhy[2];  Android_v^SPEED[1]; NONE[0];
long battMillis = -5000;
float batteryLvl = 0;
int batteryPercent = 100;
int estimatedVolume = 0;
int volDelay = 150;   // Volume shall only be changed every 100ms;
unsigned long volMillis = 0;
bool bleFlag = true;
unsigned long bleMillis = 0;
int blePerSec = 30;
int bleInterval = 1000/(float)blePerSec;


void setup() {
  
  pinMode(VBATPIN, INPUT);
  
  pinMode(POT_CS,OUTPUT);
  pinMode(POT_UD,OUTPUT);
  pinMode(POT_INC,OUTPUT);
  digitalWrite(POT_INC,HIGH);
  digitalWrite(POT_UD,LOW);
  digitalWrite(POT_CS,HIGH);

  pinMode(VOL_UP, INPUT);
  pinMode(VOL_DOWN, INPUT);
  
  Serial.begin(BAUD_RATE);

  //X9C.begin(POT_CS, POT_INC, POT_UD);
  
  
  if(!MS5611.begin(MS5611_CSB)){
    Serial.println("MS5611 failed.");
  }
  else{
    Serial.println("MS5611 success.");
  }
  
  BEEP.begin(BEEP_CTRL);
  BEEP.setClimbThreshold(SETTING.CLIMB_BEEP_TRIGGER);       //ft climbed
  BEEP.setSinkAlarmThreshold(SETTING.SINK_BEEP_TRIGGER);    //ft/s
  BEEP.setClimbPitchMax(SETTING.CLIMB_PITCH_MAX);           //Hz
  BEEP.setClimbPitchMin(SETTING.CLIMB_PITCH_MIN);           //Hz
  BEEP.setSinkPitchMax(SETTING.SINK_PITCH_MAX);             //Hz
  BEEP.setSinkPitchMin(SETTING.SINK_PITCH_MIN);             //Hz

  if(SETTING.ENABLE_BLUETOOTH){
    if(analogRead(VOL_DOWN)<500){BLUETOOTH_MODE = 1;} // Android_v^SPEED mode;
    else if(analogRead(VOL_UP)<500){BLUETOOTH_MODE = 2;} // iOS_Flyskyky mode;
    else{
      BLUETOOTH_MODE = SETTING.START_UP_BLUETOOTH_MODE; 
      if(BLUETOOTH_MODE == 0){
        SETTING.ENABLE_BLUETOOTH = false;  // START_UP_BLUETOOTH_MODE DEFINED IN "DEFAULT_SETTINGS.h";
      }
    }   
    
    ble.begin(VERBOSE_MODE);
    SWITCH_BLE_MODE(BLUETOOTH_MODE);  //INITIALIZES THE SPECIFIED BLUETOOTH MODE; 
  }
  
  batteryLvl = getBatteryLvl();
  batteryPercent = (int)(batteryLvl*156.25 - 556.25);  //conversion to percent;
  if(batteryPercent > 99){batteryPercent = 99;}
  else if(batteryPercent < 0){batteryPercent = 0;}

  adjustVolumeTo(SETTING.START_UP_VOLUME);
  estimatedVolume = SETTING.START_UP_VOLUME;

  if(BLUETOOTH_MODE==1){
    tone(BEEP_CTRL, 500, 100);
    delay(1000);
  }
  else if(BLUETOOTH_MODE==2){
    tone(BEEP_CTRL, 500, 100);
    delay(200);
    tone(BEEP_CTRL, 500, 100);
    delay(1000);
  }

  if(batteryPercent>75){
    tone(BEEP_CTRL, 300, 100);
    delay(200);
    tone(BEEP_CTRL, 400, 100);
    delay(200);
    tone(BEEP_CTRL, 500, 100);
    delay(200);
  }
  else if(batteryPercent>50){
    tone(BEEP_CTRL, 400, 100);
    delay(200);
    tone(BEEP_CTRL, 400, 100);
    delay(200);
    tone(BEEP_CTRL, 400, 100);
    delay(200);
  }
  else if(batteryPercent<25){
    tone(BEEP_CTRL, 500, 100);
    delay(200);
    tone(BEEP_CTRL, 400, 100);
    delay(200);
    tone(BEEP_CTRL, 300, 100);
    delay(200);
  }
  
}

void loop() {
  samplesThisSec++; //increment each time the loop cycles
  if(millis() - previousMillis >= 1000){ // Update value of samplesPerSec once every second:
    previousMillis=millis();
    samplesPerSec = samplesThisSec;
    samplesThisSec=0; 
    //Serial.print("BLE:"); Serial.print(ble.isConnected()); Serial.print(" ");
    //Serial.println(samplesPerSec);  //print debug info
  }


      //====BATTERY================================================================/
        //Measure battery level every 5 seconds
        if(SETTING.MEASURE_BATTERY && millis()-battMillis>=5000){
          battMillis = millis();
          batteryLvl = getBatteryLvl();
          batteryPercent = (int)(batteryLvl*156.25 - 556.25);  //conversion to percent;
          if(batteryPercent > 99){batteryPercent = 99;}
          else if(batteryPercent < 0){batteryPercent = 0;}
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
            MS5611.getPressurePa(D1_OSR),
            //MS5611_I2C.readPressure(), 
            samplesPerSec, 
            PRESSURE_FILTER_DURATION
          );
        }
        else{
          pressurePa = MS5611.getPressurePa(D1_OSR);
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
        Serial.println(altitudeFt); 
        //Serial.print(" "); 
        //Serial.println(velocityFtPerSec);     



      //====BEEP=================================================================/
      
        if(SETTING.ENABLE_BEEP && millis()>6000){
          switch(SETTING.BEEP_TYPE){
            default: BEEP.basedOnVelocity(altitudeFt, velocityFtPerSec, millis());
            break;
            case 2: BEEP.bufferedDurationIncrements(altitudeFt, velocityFtPerSec, millis());
            break;
          }
        }      



      //====BLE====================================================================/ 
        if(SETTING.ENABLE_BLUETOOTH){
          
          if(BLUETOOTH_MODE==1 && millis()-bleMillis>bleInterval){
            if(millis()>120000 && bleFlag){
              if(!ble.isConnected()){
                SETTING.ENABLE_BLUETOOTH = false;
                bleFlag = false;
                Serial.println("Disabling BLE because no connection.");
              }
              else{bleFlag = false;}
            }
            bleMillis = millis();
            receiveCommands();    //Custom Bluetooth Commands Handled When Received From Mobile Devices
            transmitVspeed();       //Custom Bluetooth Transmissions to Mobile Devices
          }
          
          else if(BLUETOOTH_MODE==2){ 
            //TODO -- Receive volume commands from Flyskyhy;
            transmitFlySkyHy(); 
          }
  
        }




      //====BUTTONS=================================================================/

        //Serial.print(" up:"); 
        //Serial.print(analogRead(VOL_UP));
        //Serial.print(" "); 
        //Serial.println(analogRead(VOL_DOWN));
        if(analogRead(VOL_UP)<500 && millis()-volMillis>=volDelay){
          volMillis = millis();
          volumeUp();
        }

        if(analogRead(VOL_DOWN)<500 && millis()-volMillis>=volDelay){
          volMillis = millis();
          volumeDown();
        }
          
  
}




void volumeUp(){
  
  //X9C.trimPot(31, X9C_UP, true);
  
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, HIGH);
  digitalWrite(POT_INC, HIGH);
  digitalWrite(POT_INC, LOW);
  digitalWrite(POT_INC, HIGH);
  digitalWrite(POT_CS,HIGH);
  
  tone(BEEP_CTRL, 400, 100);
  
  Serial.print("VOL_UP: "); 
  estimatedVolume++;
  if(estimatedVolume>32){estimatedVolume=32;}
  Serial.println(estimatedVolume);
}

void volumeDown(){

  //X9C.trimPot(31, X9C_DOWN, true);
  
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, LOW);
  digitalWrite(POT_INC, HIGH);
  digitalWrite(POT_INC, LOW);
  digitalWrite(POT_INC, HIGH);
  digitalWrite(POT_CS,HIGH); 
  
  tone(BEEP_CTRL, 400, 100);
  
  Serial.print("VOL_DOWN: ");
  estimatedVolume--;
  if(estimatedVolume<0){estimatedVolume=0;}
  Serial.println(estimatedVolume);
}

void adjustVolumeTo(int volLevel){
  
  //RESET VOLUME TO ZERO:
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, LOW);
  for(int i = 0; i <= 100; i++){
    digitalWrite(POT_INC, LOW);
    digitalWrite(POT_INC, HIGH);
  }
  digitalWrite(POT_CS,HIGH);

  //MOVE UP TO A DESIRED VOLUME:
  digitalWrite(POT_CS,LOW);
  digitalWrite(POT_UD, HIGH);
  for(int i = 0; i <= volLevel; i++){
    digitalWrite(POT_INC, LOW);
    digitalWrite(POT_INC, HIGH);
  }
  digitalWrite(POT_CS,HIGH); 
  Serial.print("VOLUME:"); Serial.println(volLevel);
}


float getBatteryLvl(){  
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}




//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//       Custom Bluetooth Commands Handled When Received From Mobile Devices;   //
//        CURRENTLY, NO BLUETOOTH COMMANDS CHANGE ANY STORED DEFAULT VALUES;    //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void receiveCommands(){
  // Check for incoming characters from Mobile Device
  ble.println("AT+BLEUARTRX");
  ble.readline(); //I think this receives something from somewhere and puts it into some buffer..?
  
  if (strcmp(ble.buffer, "OK")) {
    //Serial.print("    [Something is in the buffer]:[");
    String command = ble.buffer;
    //Serial.print(command);Serial.println("]     ");

    /*if(command == "m"){ENABLE_MS5611 = false;}
    else if(command == "M"){ENABLE_MS5611 = true;}*/

    if(command == "k"){SETTING.ENABLE_BLUETOOTH = 0;}  // Kill BLE connection
    
    if(command == "V"){SETTING.MEASURE_BATTERY=1;}        // display supply power supply voltage
    else if(command == "v"){SETTING.MEASURE_BATTERY=0;}  // display "0.00V" and don't calculate anything to improve samplesPerSec

    if(command.startsWith("a")){
      // Example: "a250"
      /*if(command == "a" || command == "a0"){ENABLE_FILTER = false;}*/
      /*else{*/
        String s = command.substring(1);
        float f = s.toFloat();
        if(f>1000){f=1000.0;}
        if(f<1){f=1; /*ENABLE_FILTER = false;*/}
        //TODO-- AVERAGING_DURATION = f; 
        /*ENABLE_FILTER = true;*/
      /*}*/
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
        tone(BEEP_CTRL, i);
        delay(5);
      }
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      noTone(BEEP_CTRL);       
    }

    if(command.startsWith("cpn")){
      // Example: "cpn300"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setClimbPitchMin(f);        //Hz
      for(int i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      for(int i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      noTone(BEEP_CTRL);      
    }

    if(command.startsWith("spx")){
      // Example: "cpx250"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setSinkPitchMax(f);        //Hz
      for(int i = BEEP.sinkPitchMin; i <= BEEP.sinkPitchMax; i+=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      for(int i = BEEP.sinkPitchMax; i >= BEEP.sinkPitchMin; i-=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      noTone(BEEP_CTRL);      
    }

    if(command.startsWith("spn")){
      // Example: "cpn200"
      String s = command.substring(3);
      float f = s.toFloat();
      BEEP.setSinkPitchMin(f);        //Hz
      for(int i = BEEP.sinkPitchMin; i <= BEEP.sinkPitchMax; i+=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      for(int i = BEEP.sinkPitchMax; i >= BEEP.sinkPitchMin; i-=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      noTone(BEEP_CTRL);
    }    

   
    if(command == "B"){  // TURN BEEP ON
      SETTING.ENABLE_BEEP = 1;
      //Serial.println("  beep:ON");
      for(float i = BEEP.pitchMin; i <= BEEP.pitchMax; i+=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      noTone(BEEP_CTRL);      
    }
    else if(command == "b"){  // TURN BEEP OFF
      for(float i = BEEP.pitchMax; i >= BEEP.pitchMin; i-=10){
        tone(BEEP_CTRL, i);
        delay(5);
      }
      noTone(BEEP_CTRL);
      //Serial.println("  beep:OFF");
      SETTING.ENABLE_BEEP = 0;       
    }      
    
    
    /*if(command == "d"){
      oled.clear(ALL);        // Clear the display's internal memory
      SETTING.ENABLE_OLED = 0;
      //Serial.println("  oled:OFF");
    }
    else if(command == "D"){
      //Serial.println("  oled:ON");
      SETTING.ENABLE_OLED = 1;    
      oled.clear(ALL);        // Clear the display's internal memory
      oled.drawBitmap(oled.logo);  // Draw v^SPEED logo
      oled.display();         // Display what's in the buffer (splashscreen)
      delay(1000);
    }*/


    /*if(command.startsWith("w")){
      // Example: "w20" (ms per loop)
      String s = command.substring(1);
      float f = s.toFloat();
      dly = (int)f;
    }*/


    /*if(command.startsWith("op")){
      // Example: "op5" (pressure OSR mode 1-5)
      String s = command.substring(2);
      float f = s.toFloat();
      D1_OSR = (int)f;
      if(D1_OSR>5){D1_OSR=5;}
      else if(D1_OSR<1){D1_OSR=1;}        
    }
    if(command.startsWith("ot")){
      // Example: "ot5" (temperature OSR mode 1-5)
      String s = command.substring(2);
      float f = s.toFloat();
      D2_OSR = (int)f;
      if(D2_OSR>5){D2_OSR=5;}
      else if(D2_OSR<1){D2_OSR=1;}
    }*/

  }
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//         Custom Bluetooth Transmissions to v^Speed Vario Android App:         //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void transmitVspeed(){
  ble.print("AT+BLEUARTTX=");
  ble.print(altitudeFt);
  ble.print("_");     
  ble.print(velocityFtPerSec);  
  ble.print("_");
  if(SETTING.MEASURE_BATTERY){ble.print(batteryLvl);}
  else{ble.print("0");}
  ble.println("V");  //Critical char used for transmission completion indication
}



//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//               Custom Bluetooth Transmissions to FlySkyHy iOS App:            //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void transmitFlySkyHy(){

  //EXAMPLE TRANSMISSION SENTENCE:
  //$LK8EX1,98684,99999,-4,28,1100,*02<CR><LF>
 
  //   where:
  //   $LK8EX1 is keyword
  //   98684 is filtered pressure in Pa relative to QNH1
  //   99999 should be altitude relative to QNH but it is ignored when pressure is available
  //   -4 is filtered vario in cm / s
  //   28 is temperature in °C
  //   1100 is battery percentage + 1000 (or 999 during charging)
  //   *02 is nmea checksum
  //   <CR><LF> CR and LF characters to terminate the line
  
  int cmPerSec = velocityFtPerSec*30.48;
  
  ble.print("AT+GATTCHAR=1,$LK8EX1,");
  ble.println(pressurePa,0);
  ble.print("AT+GATTCHAR=1,,99999,");
  ble.println(cmPerSec);
  ble.print("AT+GATTCHAR=1,,28,10");
  ble.println(batteryPercent);
  ble.print("AT+GATTCHAR=1,");
  ble.println(",*02<CR><LF>");
  
}


//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED    
//                                                                              //
//           HANDLES THE STARTUP INITIALIZATION OF THE BLUETOOTH MODULE         //
//                                                                              //
//v^SPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
void SWITCH_BLE_MODE(int bluetoothMode){
  
  //TO ENABLE FLYSKYHY TRANSMISSION MODE:
  if(bluetoothMode==2){

    //TO PERFORM FACTORY RESET:
    if( !ble.factoryReset() ){ Serial.println(" COULD NOT FACTORY RESET "); while(1); }
  
    //ADD NECESSARY ADVERTISING DATA:
    ble.sendCommandCheckOK(F( "AT+GATTCLEAR" )); 
    ble.sendCommandCheckOK(F( "AT+GAPSETADVDATA=02-01-06-11-07-1B-C5-D5-A5-02-00-03-A9-E3-11-8B-AA-A0-C6-79-E0" )); 

    //ADD A BLUETOOTH SERVICE AND CHARACTERISTIC FOR FLYSKYHY COMPATIBILITY: (Same specs as SkyDrop Vario)
    ble.sendCommandCheckOK(F( "AT+GATTADDSERVICE=UUID128=E0-79-C6-A0-AA-8B-11-E3-A9-03-00-02-A5-D5-C5-1B" )); 
    ble.sendCommandCheckOK(F( "AT+GATTADDCHAR=UUID=0xFFE1,PROPERTIES=0x10,MIN_LEN=7,MAX_LEN=20,VALUE=$LK8EX1" )); 
    
    //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
    ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=SkyDrop" )); ble.reset(); 
  
    //TO SEE WHAT BLE SERVICES AND CHARACTERISTICS ARE CURRENTLY SET:
    ble.sendCommandCheckOK(F( "AT+GATTLIST" ));
    
  }

  //TO ENABLE ANDROID TRANSMISSION MODE:
  else if(bluetoothMode==1){
    
    //TO PERFORM FACTORY RESET:
    if( !ble.factoryReset() ){ Serial.println(" COULD NOT FACTORY RESET "); while(1); }

    //TO RENAME THE DEVICE, UNCOMMENT AND EDIT THE FOLLOWING:
    ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=v^SPEED VARIO mini" )); ble.reset(); 

    //TO SEE WHAT BLE SERVICES AND CHARACTERISTICS ARE CURRENTLY SET:
    ble.sendCommandCheckOK(F( "AT+GATTLIST" ));
  
  }
  
}
