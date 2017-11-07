#include "DEFAULT_SETTINGS.h"
#include "OLED.h"
#include "MENU.h"
#include "BUTTON.h"

#define BAUD_RATE                 115200    // Serial Monitor baud rate
#define BUTTON_PIN                    A1
#define OLED_DC                       10    // Data/Command Pin
#define OLED_CS                       11    // Chip/Slave Select Pin
#define OLED_RST                      12    // Reset Pin

BUTT butt;
Default SETTING;
MENU THE_MENU;
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);

void setup() {
  Serial.begin(BAUD_RATE);
  //while(!Serial);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if(SETTING.ENABLE_OLED){ 
    oled.begin();           // Initialize the OLED
    oled.clear(ALL);        // Clear the display's internal memory
    oled.setFontType(0);
    oled.display();         // Display what's in the buffer (splashscreen)
  }
}

void loop() {
  

  unsigned long currentMillis = millis();

      /*#######################################*/
     ////////////////MAIN ACIVITY//////////////
    /*######################################*/
    if(THE_MENU.CURRENT_PAGE==THE_MENU.MAIN_ACTIVITY){
      oled.clear(PAGE);  //Clear the screen
      oled.line(random(0,64), random(0,48), random(0,64), random(0,48));
      butt.buttonPress=butt.checkButton(BUTTON_PIN, currentMillis);
      if(butt.buttonPress==butt.BUTTON_CLICK){
        butt.buttonPress=butt.BUTTON_NO_ACTION; 
        SETTING.ENABLE_BEEP=!SETTING.ENABLE_BEEP;
        oled.setCursor(0,0);
        if(!SETTING.ENABLE_BEEP){oled.print("BEEP=OFF");}
        else{oled.print("BEEP=ON");}
        oled.display();
        delay(250);
      }  
      if(butt.buttonPress==butt.BUTTON_HOLD){
        butt.buttonPress=butt.BUTTON_NO_ACTION; 
        THE_MENU.CURRENT_PAGE=THE_MENU.SETTINGS;
      }
      oled.setCursor(10,20);
      oled.println("MAIN");
      oled.setCursor(5, 30);
      oled.println("ACTIVITY");
      oled.display();   //Draw the new screen
    }


      /*#######################################*/
     ////////////////SETTINGS MENU//////////////
    /*######################################*/    
    String _SETTINGS =      "SETTINGS*";
    String _BEEP =          "  >BEEP";
    String _BLUETOOTH =     "  >BLE";
    String _DISPLAY =       "  >OLED";
    String _USER =          "  >USER";
    String _EXIT =          "  EXIT";
    while(THE_MENU.CURRENT_PAGE==THE_MENU.SETTINGS){
      unsigned long buttMillis = millis();
      butt.buttonPress=butt.checkButton(BUTTON_PIN, buttMillis);
      //IF A BUTTON IS CLICKED, HIGHLIGHT THE NEXT ITEM:
      if(butt.buttonPress==butt.BUTTON_CLICK){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_BEEP.endsWith("*")){_BEEP.replace('*',' '); _BEEP.trim(); _BEEP="  "+_BEEP; _BLUETOOTH+='*';}
        else if(_BLUETOOTH.endsWith("*")){_BLUETOOTH.replace('*',' '); _BLUETOOTH.trim(); _BLUETOOTH="  "+_BLUETOOTH; _DISPLAY+='*';}
        else if(_DISPLAY.endsWith("*")){_DISPLAY.replace('*',' '); _DISPLAY.trim(); _DISPLAY="  "+_DISPLAY; _USER+='*';}
        else if(_USER.endsWith("*")){_USER.replace('*',' '); _USER.trim(); _USER="  "+_USER; _EXIT+='*';}
        else if(_EXIT.endsWith("*")){_EXIT.replace('*',' '); _EXIT.trim(); _EXIT="  "+_EXIT; _SETTINGS+='*';}
        else if(_SETTINGS.endsWith("*")){_SETTINGS.replace('*',' '); _SETTINGS.trim(); _BEEP+='*';}
      }
      //IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
      if(butt.buttonPress==butt.BUTTON_HOLD){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_BEEP.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.BEEP;}
        else if(_BLUETOOTH.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.BLUETOOTH;}
        else if(_DISPLAY.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.OLED;}
        else if(_USER.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.USER;}
        else if(_EXIT.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.MAIN_ACTIVITY;}
        else if(_SETTINGS.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.MAIN_ACTIVITY;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_SETTINGS);
      //oled.setCursor(0,10);
      oled.println(_BEEP);
      //oled.setCursor(0,20);
      oled.println(_BLUETOOTH);
      oled.println(_DISPLAY);
      oled.println(_USER);
      oled.println(_EXIT);
      oled.display();
    }



      /*#######################################*/
     /////////////////BEEP MENU////////////////
    /*######################################*/  
           _BEEP =          "BEEP";
    String _VOLUME =        "  >VOL*";
    String _THRESHOLD =     "  >THRSH";
    String _PITCH =         "  >PITCH";
    String _BLANK =         "  ";
    String _SAVEANDEXIT =   "  EXIT";
    while(THE_MENU.CURRENT_PAGE==THE_MENU.BEEP){
      unsigned long buttMillis = millis();
      butt.buttonPress=butt.checkButton(BUTTON_PIN, buttMillis);
      //IF A BUTTON IS CLICKED, HIGHLIGHT THE NEXT ITEM:
      if(butt.buttonPress==butt.BUTTON_CLICK){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_VOLUME.endsWith("*")){_VOLUME.replace('*',' '); _VOLUME.trim(); _VOLUME="  "+_VOLUME; _THRESHOLD+='*';}
        else if(_THRESHOLD.endsWith("*")){_THRESHOLD.replace('*',' '); _THRESHOLD.trim(); _THRESHOLD="  "+_THRESHOLD; _PITCH+='*';}
        else if(_PITCH.endsWith("*")){_PITCH.replace('*',' '); _PITCH.trim(); _PITCH="  "+_PITCH; _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _BEEP+='*';}
        else if(_BEEP.endsWith("*")){_BEEP.replace('*',' '); _BEEP.trim(); _VOLUME+='*';}
      }
      //IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
      if(butt.buttonPress==butt.BUTTON_HOLD){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_VOLUME.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.VOLUME;}
        else if(_THRESHOLD.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.THRESHOLD;}
        else if(_PITCH.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.PITCH;}
        else if(_SAVEANDEXIT.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.MAIN_ACTIVITY;}
        else if(_BEEP.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.SETTINGS;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_BEEP);
      oled.println(_VOLUME);
      oled.println(_THRESHOLD);
      oled.println(_PITCH);
      oled.println(_BLANK);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }


      /*#######################################*/
     ////////////////VOLUME MENU///////////////
    /*######################################*/  
           _VOLUME =       "VOL=100";
    String _UP25 =         "  +25%*";
    String _DOWN25 =       "  -25%";
    String _MUTE =         "  OFF";
    String _UNMUTE =       "->ON";
           _SAVEANDEXIT =  "  EXIT";
    if(!SETTING.ENABLE_BEEP){  _MUTE="->OFF"; _UNMUTE="  ON";}
    if(SETTING.ENABLE_BEEP){   _MUTE="  OFF"; _UNMUTE="->ON";} 
    while(THE_MENU.CURRENT_PAGE==THE_MENU.VOLUME){
      if(_VOLUME.endsWith("*")){_VOLUME = "VOL="+String(SETTING.VOLUME); _VOLUME+="%"; _VOLUME+="*";}
      else{_VOLUME = "VOL="+String(SETTING.VOLUME); _VOLUME+="%";}  
      unsigned long buttMillis = millis();
      butt.buttonPress=butt.checkButton(BUTTON_PIN, buttMillis);
      //BUTTON IS CLICKED; DO THINGS:
      if(butt.buttonPress==butt.BUTTON_CLICK){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_UP25.endsWith("*")){_UP25.replace('*',' '); _UP25.trim(); _UP25="  "+_UP25; _DOWN25+='*';}
        else if(_DOWN25.endsWith("*")){_DOWN25.replace('*',' '); _DOWN25.trim(); _DOWN25="  "+_DOWN25; _MUTE+='*';}
        else if(_MUTE.endsWith("*")){_MUTE.replace('*',' '); _MUTE.trim(); if(_UNMUTE.startsWith("->")){_MUTE="  "+_MUTE;} _UNMUTE+='*';}
        else if(_UNMUTE.endsWith("*")){_UNMUTE.replace('*',' '); _UNMUTE.trim(); if(_MUTE.startsWith("->")){_UNMUTE="  "+_UNMUTE;} _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _VOLUME+='*';}
        else if(_VOLUME.endsWith("*")){_VOLUME.replace('*',' '); _VOLUME.trim(); _UP25+='*';}
      }
      //BUTTON IS HELD FOR MORE THAN ONE SECOND; DO THINGS:
      if(butt.buttonPress==butt.BUTTON_HOLD){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        
        if(_UP25.endsWith("*")){_VOLUME=_VOLUME.substring(4); int _VOL=_VOLUME.toFloat(); _VOL+=25; if(_VOL>100){_VOL=100;} SETTING.VOLUME=_VOL; _VOLUME="VOL="+String(_VOL); _VOLUME+="%"; /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_DOWN25.endsWith("*")){_VOLUME=_VOLUME.substring(4); int _VOL=_VOLUME.toFloat(); _VOL-=25; if(_VOL<0){_VOL=0;} SETTING.VOLUME=_VOL; _VOLUME="VOL="+String(_VOL); _VOLUME+="%"; /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_MUTE.endsWith("*")){ if(_UNMUTE.startsWith("->")){_UNMUTE="  "+_UNMUTE.substring(2); _MUTE="->OFF*";} SETTING.ENABLE_BEEP=0; /*STORAGE.storeVariable(STORAGE.search_ENABLE_BEEP, SETTING.ENABLE_BEEP);*/}
        else if(_UNMUTE.endsWith("*")){ if(_MUTE.startsWith("->")){_MUTE="  "+_MUTE.substring(2); _UNMUTE="->ON*";} SETTING.ENABLE_BEEP=1; /*STORAGE.storeVariable(STORAGE.search_ENABLE_BEEP, SETTING.ENABLE_BEEP);*/}
        else if(_SAVEANDEXIT.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.MAIN_ACTIVITY;}
        else if(_VOLUME.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.BEEP;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_VOLUME);
      oled.println(_UP25);
      oled.println(_DOWN25);
      oled.println(_MUTE);
      oled.println(_UNMUTE);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }


      /*#######################################*/
     ///////////////THRESHOLD MENU/////////////
    /*######################################*/   
           _THRESHOLD =     "THRESHLD";
    String _CLIMB =         "  >CLIMB*";
    String _SINK =          "  >SINK";
           _BLANK =         "  ";
           _BLANK =         "  ";
           _SAVEANDEXIT =   "  EXIT";  
    while(THE_MENU.CURRENT_PAGE==THE_MENU.THRESHOLD){
      unsigned long buttMillis = millis();
      butt.buttonPress=butt.checkButton(BUTTON_PIN, buttMillis);
      //IF A BUTTON IS CLICKED, HIGHLIGHT THE NEXT ITEM:
      if(butt.buttonPress==butt.BUTTON_CLICK){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_CLIMB.endsWith("*")){_CLIMB.replace('*',' '); _CLIMB.trim(); _CLIMB="  "+_CLIMB; _SINK+='*';}
        else if(_SINK.endsWith("*")){_SINK.replace('*',' '); _SINK.trim(); _SINK="  "+_SINK; _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _THRESHOLD+='*';}
        else if(_THRESHOLD.endsWith("*")){_THRESHOLD.replace('*',' '); _THRESHOLD.trim(); _CLIMB+='*';}
      }
      //IF A BUTTON IS HELD FOR MORE THAN ONE SECOND, SELECT/TOGGLE THE ITEM:
      if(butt.buttonPress==butt.BUTTON_HOLD){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_CLIMB.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.CLIMB;}
        else if(_SINK.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.SINK;}
        else if(_SAVEANDEXIT.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.MAIN_ACTIVITY;}
        else if(_THRESHOLD.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.BEEP;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_THRESHOLD);
      oled.println(_CLIMB);
      oled.println(_SINK);
      oled.println(_BLANK);
      oled.println(_BLANK);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }

      /*#######################################*/
     ////////////////CLIMB MENU////////////////
    /*######################################*/    
           _CLIMB =       "CLIMB=1";
    String _UP1FT =         "  +1ft*";
    String _DOWN1FT =       "  -1ft";
           _BLANK =         "  ";
           _BLANK =         "  ";
           _SAVEANDEXIT =  "  EXIT";     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.CLIMB){
      if(_CLIMB.endsWith("*")){_CLIMB = "CLIMB="+String(round(SETTING.CLIMB_BEEP_TRIGGER)); _CLIMB+="*";}
      else{_CLIMB = "CLIMB="+String(round(SETTING.CLIMB_BEEP_TRIGGER));}  
      unsigned long buttMillis = millis();
      butt.buttonPress=butt.checkButton(BUTTON_PIN, buttMillis);
      //BUTTON IS CLICKED; DO THINGS:
      if(butt.buttonPress==butt.BUTTON_CLICK){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        if(_UP1FT.endsWith("*")){_UP1FT.replace('*',' '); _UP1FT.trim(); _UP1FT="  "+_UP1FT; _DOWN1FT+='*';}
        else if(_DOWN1FT.endsWith("*")){_DOWN1FT.replace('*',' '); _DOWN1FT.trim(); _DOWN1FT="  "+_DOWN1FT; _SAVEANDEXIT+='*';}
        else if(_SAVEANDEXIT.endsWith("*")){_SAVEANDEXIT.replace('*',' '); _SAVEANDEXIT.trim(); _SAVEANDEXIT="  "+_SAVEANDEXIT; _CLIMB+='*';}
        else if(_CLIMB.endsWith("*")){_CLIMB.replace('*',' '); _CLIMB.trim(); _UP1FT+='*';}
      }
      //BUTTON IS HELD FOR MORE THAN ONE SECOND; DO THINGS:
      if(butt.buttonPress==butt.BUTTON_HOLD){
        butt.buttonPress=butt.BUTTON_NO_ACTION;
        
        if(_UP1FT.endsWith("*")){_CLIMB=_CLIMB.substring(6); int _CLM=_CLIMB.toFloat(); _CLM+=1; if(_CLM>10){_CLM=10;} SETTING.CLIMB_BEEP_TRIGGER=_CLM; _CLIMB="CLIMB="+String(round(_CLM));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_DOWN1FT.endsWith("*")){_CLIMB=_CLIMB.substring(6); int _CLM=_CLIMB.toFloat(); _CLM-=1; if(_CLM<1){_CLM=1;} SETTING.CLIMB_BEEP_TRIGGER=_CLM; _CLIMB="CLIMB="+String(round(_CLM));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
        else if(_SAVEANDEXIT.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.MAIN_ACTIVITY;}
        else if(_CLIMB.endsWith("*")){THE_MENU.CURRENT_PAGE=THE_MENU.THRESHOLD;}
      }
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.println(_CLIMB);
      oled.println(_UP1FT);
      oled.println(_DOWN1FT);
      oled.println(_BLANK);
      oled.println(_BLANK);
      oled.println(_SAVEANDEXIT);
      oled.display();
    }


      /*#######################################*/
     /////////////////SINK MENU////////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.SINK){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }


      /*#######################################*/
     ////////////////PITCH MENU////////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.PITCH){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }


      /*#######################################*/
     ///////////////CLIMB_MAX MENU/////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.CLIMB_MAX){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }


      /*#######################################*/
     ///////////////CLIMB_MIN MENU/////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.CLIMB_MIN){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }    


      /*#######################################*/
     ////////////////SINK_MAX MENU/////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.SINK_MAX){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }      


      /*#######################################*/
     ////////////////SINK_MIN MENU/////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.SINK_MIN){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }           
        
    
      /*#######################################*/
     ///////////////BLUETOOTH MENU/////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.BLUETOOTH){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }         


      /*#######################################*/
     /////////////////OLED MENU////////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.OLED){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     //////////////CHART_SPEED MENU////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.CHART_SPEED){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     ////////////////SINK_MAX MENU/////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.SINK_MAX){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     



      /*#######################################*/
     //////////////////USER MENU///////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.USER){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     ///////////////EDIT_NAMES MENU////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.EDIT_NAMES){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     


      /*#######################################*/
     ////////////////USER_1 MENU///////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.USER_1){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }     
    

      /*#######################################*/
     ////////////////USER_2 MENU///////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.USER_2){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }



      /*#######################################*/
     ////////////////USER_3 MENU///////////////
    /*######################################*/     
    while(THE_MENU.CURRENT_PAGE==THE_MENU.USER_3){
        //TODO--DISPLAY MENU FOR CHANGING OPTIONS;
    }





    
}
