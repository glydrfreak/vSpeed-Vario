#include "MICRO_SD.h"
#include "DEFAULT_SETTINGS.h"

Default VAR;


  //Serial.begin(115200);
  //while(!Serial);
  //Serial.println("initializing SD...");



  //SD INITIALIZATION:
  //if (!SD.begin(SD_PIN)) {Serial.println("initialization failed!"); return;}



  //REMOVE A FILE:
  //SD.remove(SETTINGS);

  

void MEMORY::storeVariable(String variableToStore, float valueToStore){
  
  //OPEN A FILE FOR WRITING:
  myFile = SD.open(USER_SETTINGS_FILE, FILE_WRITE);
  if (myFile) {
    String dataString = "";
    char dataChar[20] = {};
    int i = 0;

    //READ THE ENTIRE FILE:
    while (myFile.available()) {
      dataChar[i] = myFile.read();
      dataString.concat(dataChar[i]);

      //LOOK FOR THE SPECIFIED VARIABLE AND REPLACE VALUE:
      if(dataString.startsWith(variableToStore)){
        myFile.print(valueToStore);
        myFile.print("          ");
      }
    
      //RESET BEFORE LOOKING ON THE NEXT LINE:
      if(dataChar[i]=='\n'){
        Serial.println(dataString);
        dataString = ""; 
        i = -1;
      }
      i++;
      
    } 
    myFile.close();
    
  }
  else {Serial.print("error opening ");Serial.println(USER_SETTINGS_FILE);}
  
}




float MEMORY::findVariable(String variableToFind){

  //CREATE NEW USER SETTINGS FILE IF THE SPECIFIED USER DOES NOT HAVE ONE:
  if(!SD.exists(USER_SETTINGS_FILE)){
    Serial.print("Creating User Settings file for: "); 
    Serial.print(USER); 
    Serial.println("."); 
    newUser(USER);
  }
  
  //OPEN A FILE FOR READING:
  myFile = SD.open(USER_SETTINGS_FILE);
  if (myFile) {
    String dataString = "";
    char dataChar[20] = {};
    int i = 0;

    //READ THE ENTIRE FILE:
    while (myFile.available()) {
      dataChar[i] = myFile.read();
      dataString.concat(dataChar[i]);

      //LOOK FOR THE SPECIFIED VARIABLE:
      if(dataString.startsWith(variableToFind)){
        String s = dataString.substring(variableToFind.length());
        return s.toFloat();
      }
    
      //RESET BEFORE LOOKING ON THE NEXT LINE:
      if(dataChar[i]=='\n'){
        Serial.println(dataString);
        dataString = ""; 
        i = -1;
      }
      i++;
      
    }
    myFile.close();
  }
  else {Serial.print("error opening ");Serial.println(USER_SETTINGS_FILE);}

  Serial.println("Could not find variable.");
  return 0;
}






void MEMORY::newUser(String userName){

  //AUTOMATICALLY SWITCH TO NEW USER:
  USER = userName;
  USER_SETTINGS_FILE = USER+".TXT";

  //REMOVE AND REWRITE A NEW FILE IF THE SPECIFIED USER ALREADY EXISTS:
  if(SD.exists(USER_SETTINGS_FILE)){
    SD.remove(USER_SETTINGS_FILE);
  }
  
  //CREATE A FILE FOR WRITING:
  myFile = SD.open(USER_SETTINGS_FILE, FILE_WRITE);
  if (myFile) {


    //WRITE DEFAULT SETTINGS TO NEW USER FILE:
    myFile.seek(0);
    
    myFile.println("");
    myFile.println("//USER:");
    myFile.print("String USER = \""); myFile.print(USER); myFile.println("\";");

    myFile.println("");
    myFile.println("//DEFAULT [1]YES, [0]NO:");
    myFile.print("bool ENABLE_BLUETOOTH = "); myFile.print(VAR.ENABLE_BLUETOOTH); myFile.println(";");
    
    myFile.println("");
    myFile.println("//DEFAULT [1]VSPEED_ANDROID, [2]FLYSKYHY_IOS:");
    myFile.print("int BLUETOOTH_MODE = "); myFile.print(VAR.BLUETOOTH_MODE); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT [1]YES, [0]NO:");
    myFile.print("bool ENABLE_OLED = "); myFile.print(VAR.ENABLE_OLED); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT 30 seconds OF VELOCITY DISPLAYED ON CHART:");
    myFile.print("float CHART_SPEED = "); myFile.print(VAR.CHART_SPEED); myFile.println(";");
    
    myFile.println("");
    myFile.println("//[1]YES, [0]NO:");
    myFile.print("bool ENABLE_BEEP = "); myFile.print(VAR.ENABLE_BEEP); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT 100%:");
    myFile.print("int VOLUME = "); myFile.print(VAR.VOLUME); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT [1]BASED_ON_VELOCITY, [2]BUFFERED_INCREMENTS:");
    myFile.print("int BEEP_TYPE = "); myFile.print(VAR.BEEP_TYPE); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT 1.0 ft:");
    myFile.print("float CLIMB_BEEP_TRIGGER = "); myFile.print(VAR.CLIMB_BEEP_TRIGGER); myFile.println(";");  

    myFile.println("");
    myFile.println("//DEFAULT -1.0 ft/s:");
    myFile.print("float SINK_ALARM_TRIGGER = "); myFile.print(VAR.SINK_ALARM_TRIGGER); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT 500.0 Hz:");
    myFile.print("float CLIMB_PITCH_MAX = "); myFile.print(VAR.CLIMB_PITCH_MAX); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT 300.0 Hz:");
    myFile.print("float CLIMB_PITCH_MIN = "); myFile.print(VAR.CLIMB_PITCH_MIN); myFile.println(";");
      
    myFile.println("");
    myFile.println("//DEFAULT 250.0 Hz:");
    myFile.print("float SINK_PITCH_MAX = "); myFile.print(VAR.SINK_PITCH_MAX); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT 150.0 Hz:");
    myFile.print("float SINK_PITCH_MIN = "); myFile.print(VAR.SINK_PITCH_MIN); myFile.println(";");

    myFile.println("");
    myFile.println("//DEFAULT [1]YES, [0]NO:");
    myFile.print("bool MEASURE_BATTERY = "); myFile.print(VAR.MEASURE_BATTERY); myFile.println(";");
    
    
    myFile.close();

//====NEW FILE SHOULD CONTAIN:====================================================
//    
//    //USER:
//    String USER = "userName";
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool ENABLE_BLUETOOTH = 1;
//    
//    //DEFAULT [1]VSPEED_ANDROID, [2]FLYSKYHY_IOS:
//    int BLUETOOTH_MODE = 1;
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool ENABLE_OLED = 1;
//    
//    //DEFAULT 30 seconds OF VELOCITY DISPLAYED ON CHART:
//    float CHART_SPEED = 30;
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool ENABLE_BEEP = 1;
//
//    //DEFAULT 100%:
//    int VOLUME = 100;
//    
//    //DEFAULT [1]BASED_ON_VELOCITY, [2]BUFFERED_INCREMENTS:
//    int BEEP_TYPE = 1;
//    
//    //DEFAULT 1.0 ft:
//    float CLIMB_BEEP_TRIGGER = 1.0;
//    
//    //DEFAULT -1.0 ft/s:
//    float SINK_ALARM_TRIGGER = -1.0;
//    
//    //DEFAULT 500.0 Hz:
//    float CLIMB_PITCH_MAX = 500.0;
//    
//    //DEFAULT 300.0 Hz:
//    float CLIMB_PITCH_MIN = 300.0;
//    
//    //DEFAULT 250.0 Hz:
//    float SINK_PITCH_MAX = 250.0;
//    
//    //DEFAULT 150.0 Hz:
//    float SINK_PITCH_MIN = 150.0;
//    
//    //DEFAULT [1]YES, [0]NO:
//    bool MEASURE_BATTERY = 1;
//
//================================================================================
        
    
  }
  else {Serial.print("error creating ");Serial.println(USER_SETTINGS_FILE);}
  

}

