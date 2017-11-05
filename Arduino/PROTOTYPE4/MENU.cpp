#include "MENU.h"
#include "MICRO_SD.h"
#include "DEFAULT_SETTINGS.h"
#include "BUTTON.h"

BUTT BUTT;
MEMORY MEMORY;
Default Setting;

/*float MENU::UPDATE(int buttonAction){
  switch(buttonAction){
    
    case BUTT.BUTTON_NO_ACTION: 
      break;
      
    case BUTT.BUTTON_CLICK: 
      switch(CURRENT_PAGE){

        case page.MAIN_ACTIVITY: 
            //MUTE IF UNMUTED:
            if(MEMORY.findVariable(MEMORY.search_ENABLE_BEEP)){
              Setting.ENABLE_BEEP = 0;
              MEMORY.storeVariable(MEMORY.search_ENABLE_BEEP, 0); 
            }
            //UNMUTE IF MUTED:
            else if(!MEMORY.findVariable(MEMORY.search_ENABLE_BEEP)){
              Setting.ENABLE_BEEP = 1;
              MEMORY.storeVariable(MEMORY.search_ENABLE_BEEP, 1);
            }
          break;
          
        case page.SETTINGS:
            //HIGHLIGHT NEXT ITEM;
          break;
          
        case page.BEEP:
            //HIGHLIGHT NEXT ITEM;
          break;
          
        case page.VOLUME:
            //HIGHLIGHT NEXT ITEM;
          break;
        
        //...
          
        default: 
          break;
      };
      break;
    
    case BUTT.BUTTON_HOLD:  
      break;
  
  };
  

  return 0;
}*/


            /*//MUTE IF UNMUTED:
            if(MEMORY.findVariable(MEMORY.search_ENABLE_BEEP)){
              Setting.ENABLE_BEEP = 0;
              MEMORY.storeVariable(MEMORY.search_ENABLE_BEEP, 0); 
            }
            //UNMUTE IF MUTED:
            else if(!MEMORY.findVariable(MEMORY.search_ENABLE_BEEP)){
              Setting.ENABLE_BEEP = 1;
              MEMORY.storeVariable(MEMORY.search_ENABLE_BEEP, 1);
            }*/
