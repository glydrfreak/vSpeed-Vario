//#include "MENU.h"
//#include "DEFAULT_SETTINGS.h"
//#include "BUTTON.h"
//
//
//BUTT BUTT;
//Default UserSetting;
////#include "OLED.h"
////#define OLED_DC                       10    // Data/Command Pin
////#define OLED_CS                       11    // Chip/Slave Select Pin
////#define OLED_RST                      12    // Reset Pin
////MicroOLED oled(OLED_RST, OLED_DC, OLED_CS);
//
//
//void MENU_ACTIVITY(
//  int INTENDED_PAGE, 
//  int PREVIOUS_PAGE,
//  int displayedValue, 
//  int displayedValueIncrement,
//  int displayedValueMin, 
//  int displayedValueMax, 
//  int itemOneType, String itemOne, int itemOneNextPage,
//  int itemTwoType, String itemTwo, int itemTwoNextPage,
//  int itemThreeType, String itemThree, int itemThreeNextPage,
//  int itemFourType, String itemFour, int itemFourNextPage,
//  int itemFiveType, String itemFive, int itemFiveNextPage
//){
//
//  
//  const int numberOfItems = 7;
//  String ITEM[] = {};
//  
//  ITEM[numberOfItems+1] = {
//    "PLACE_HOLDER", 
//    itemOne, 
//    itemTwo, 
//    itemThree, 
//    itemFour, 
//    itemFive, 
//    "<-", 
//    "->"
//  };
//  
//  int TYPE[numberOfItems+1] = {
//    0,//PLACE_HOLDER;
//    itemOneType, 
//    itemTwoType, 
//    itemThreeType, 
//    itemFourType, 
//    itemFiveType, 
//    BACK, 
//    EXIT
//  };
//  
//  int NEXT[numberOfItems+1] = {
//    0,//PLACE_HOLDER;
//    itemOneNextPage, 
//    itemTwoNextPage, 
//    itemThreeNextPage, 
//    itemFourNextPage, 
//    itemFiveNextPage, 
//    PREVIOUS_PAGE, 
//    MAIN_ACTIVITY
//  };
//
//  while(CURRENT_PAGE==INTENDED_PAGE){
//    
//    //TODO--OBTAIN NECESSARY DEFAULT OR EXISTING VALUES:
//    /*if(itemOne.endsWith("*")){itemOne = "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER)); itemOne+="*";}  */
//    /*else{itemOne = "SINK="+String(round(SETTING.SINK_ALARM_TRIGGER));}  */
//
//    //CHECK BUTTON STATUS:
//    BUTT.buttonPress=BUTT.checkButton(BUTT.BUTTON_PIN, millis());
//    
//    //BUTTON WAS CLICKED; DO THINGS:
//    if(BUTT.buttonPress==BUTT.BUTTON_CLICK){
//      BUTT.buttonPress=BUTT.BUTTON_NO_ACTION;
//      for(int i=1; i<=numberOfItems; i++){
//        if(ITEM[i].endsWith("*")){
//          ITEM[i] = ITEM[i].substring(0,ITEM[i].length());
//          if(i<=numberOfItems-1){ITEM[i+1]+='*';}
//          else{ITEM[1]+='*';}
//        }
//      }
//    }
//    
//    //FOR ALL ITEM TYPES THAT SHOULD NOT HAVE A CURSOR:
//    for(int i=1; i<=numberOfItems; i++){
//      if(ITEM[i].endsWith("*") && (TYPE[i]==BLANK || TYPE[i]==TITLE || TYPE[i]==DISPLAYED_VALUE)){
//        ITEM[i] = ITEM[i].substring(0,ITEM[i].length()); 
//        if(i<=numberOfItems-1){ITEM[i+1] += '*';}
//        else{ITEM[1] += '*';}
//      }
//    }
//    
//    
//    //BUTTON WAS HELD; DO THINGS:
//    if(BUTT.buttonPress==BUTT.BUTTON_HOLD){
//      BUTT.buttonPress=BUTT.BUTTON_NO_ACTION;
      
//      //LOOK AT EACH ITEM IN THE LIST:
//      for(int i=1; i<=numberOfItems; i++){
//        //IDENTIFY THE ITEM THAT HAS THE CURSOR:
//        if(ITEM[i].endsWith("*")){
//          //IDENTIFY THE ITEM TYPE:
//          switch(TYPE[i]){
//            //UPDATE DISPLAYED_VALUE:
//            case 3/*CLICKABLE*/: 
//            _lengthNoVal=ITEM[i].indexOf("=")+1; 
//            _noVAL=ITEM[i].substring(0,ITEM[i].indexOf("=")+1); 
//            if(_noVAL.startsWith("+")){
//              VAL+=displayedValueIncrement; 
//              if(VAL>displayedValueMax){VAL=displayedValueMax;} 
//            }
//            else if(_noVAL.startsWith("-")){
//              VAL-=displayedValueIncrement; 
//              if(VAL<displayedValueMin){VAL=displayedValueMin;} 
//            }
//
//            //TODO-- STORE THE NEW VALUE:
//            /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/
//            break;
//            
//            //GO TO NEXT PAGE:
//            case 4/*EXPANDABLE*/: CURRENT_PAGE = NEXT[i]; 
//            break;   
//
//            //MARK CURRENT ITEM AND UN-MARK SPECIFIED OTHER:
//            case 5/*CHECKABLE*/: 
//            break;
//            
//            //GO TO PREVIOUS PAGE:
//            //case BACK: CURRENT_PAGE = PREVIOUS_PAGE; 
//            //break;   
//            
//            //GO TO MAIN ACTIVITY:
//            //case EXIT: CURRENT_PAGE = MAIN_ACTIVITY; 
//            //break; 
//            
//          };
//        }
//      }
//
//      //TO UPDATE DISPLAYED_VALUE, DO SOMETHING SIMILAR TO THE FOLLOWING:
//      /**/
//      //if(_LINE2.endsWith("*")){int _lengthNoVal=_LINE1.indexOf("=")+1; String _noVAL=_LINE1.substring(0,_lengthNoVal); String _sVAL=_LINE1.substring(_lengthNoVal); int _VAL=_sVAL.toFloat(); _VAL+=1; if(_VAL>-1){_VAL=-1;} SETTING.SINK_ALARM_TRIGGER=_VAL; _LINE1=_noVAL+String(round(_VAL));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
//      //if(_LINE3.endsWith("*")){int _lengthNoVal=_LINE1.indexOf("=")+1; String _noVAL=_LINE1.substring(0,_lengthNoVal); String _sVAL=_LINE1.substring(_lengthNoVal); int _VAL=_sVAL.toFloat(); _VAL-=1; if(_VAL<-10){_VAL=-10;} SETTING.SINK_ALARM_TRIGGER=_VAL; _LINE1=_noVAL+String(round(_VAL));  /*STORAGE.storeVariable(STORAGE.search_VOLUME, SETTING.VOLUME);*/}
//      /**/
//
//    }
//
//    //REDRAW THE PAGE:
//    oled.clear(PAGE);
//    oled.setCursor(0,0);
//    for(int i=1; i<=numberOfItems; i++){
//      if(i<=numberOfItems-2){oled.println(ITEM[i]);}
//      else{
//        oled.setCursor(0,48); oled.print(ITEM[7]);
//        oled.setCursor(36,48); oled.print(ITEM[8]);
//      }
//    }
//    oled.display();
//  }  
//  
//
//}
