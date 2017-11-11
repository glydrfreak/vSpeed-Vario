#include "MENU.h"
#include "BUTTON.h"

BUTTON _BUTTON;





void MENU::menuItem(String item1, String item2, String item3, String item4, String item5, String item6, String item7){
  ITEM[1] = item1;
  ITEM[2] = item2;
  ITEM[3] = item3;
  ITEM[4] = item4;
  ITEM[5] = item5;
  ITEM[6] = item6;
  ITEM[7] = item7;
}





void MENU::itemPurpose(int purpose1, int purpose2, int purpose3, int purpose4, int purpose5, int purpose6, int purpose7){
  PURPOSE[1] = purpose1;
  PURPOSE[2] = purpose2;
  PURPOSE[3] = purpose3;
  PURPOSE[4] = purpose4;
  PURPOSE[5] = purpose5;
  PURPOSE[6] = purpose6;
  PURPOSE[7] = purpose7;
}







void MENU::ifSelectedGoTo(int activity1, int activity2, int activity3, int activity4, int activity5, int activity6, int activity7){
  ACTIVITY[1] = activity1;
  ACTIVITY[2] = activity2;
  ACTIVITY[3] = activity3;
  ACTIVITY[4] = activity4;
  ACTIVITY[5] = activity5;
  ACTIVITY[6] = activity6;
  ACTIVITY[7] = activity7;
}







void MENU::integerAdjust(int integerMin, int integerMax){
  INT_ADJUST[0] = integerMin;
  INT_ADJUST[1] = integerMax;
}






void MENU::floatAdjust(float floatMin, float floatMax){
  FLOAT_ADJUST[0] = floatMin;
  FLOAT_ADJUST[1] = floatMax;
}







void MENU::booleanToggle(String displayForTrue, String displayForFalse){
  BOOL_DISPLAYED[0] = displayForFalse;
  BOOL_DISPLAYED[1] = displayForTrue;
}








void MENU::initializeActivity(int& intToAdjust, float& floatToAdjust, bool& boolToAdjust){
  
  //Serial.print("CURRENT_PAGE=="); Serial.println(CURRENT_PAGE);
  
  enum {_MIN, _MAX};

  //INITIALIZE NAMED_INT, NAMED_FLOAT, AND BOOL_TOGGLER TO PROPER VALUE:
  for(int i=1; i<=numberOfItems; i++){
    if(PURPOSE[i]==NAMED_INT){
      ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(intToAdjust);
    }
    if(PURPOSE[i]==NAMED_FLOAT){
      ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(floatToAdjust);
    }
    if(PURPOSE[i]==BOOL_TOGGLER){
      if(ITEM[i].endsWith("*")){
        if(boolToAdjust){ITEM[i] = BOOL_DISPLAYED[1]+"*";}
        else if(!boolToAdjust){ITEM[i] = BOOL_DISPLAYED[0]+"*";}
      }
      else{
        if(boolToAdjust){ITEM[i] = BOOL_DISPLAYED[1];}
        else if(!boolToAdjust){ITEM[i] = BOOL_DISPLAYED[0];}
      }
      
    }
    
    if(PURPOSE[i]==ITEM_SWITCHER){
      for(int j=1; j<=numberOfItems; j++){
        if(PURPOSE[j]==ITEM_SWITCHER && ITEM[j].startsWith("*")){
          ITEM[j].trim();
          ITEM[j] = " "+ITEM[j].substring(1);
        }
      }      
      ITEM[intToAdjust+1].trim();
      ITEM[intToAdjust+1] = "*"+ITEM[intToAdjust+1];
    }
    
  }

  //CHECK WHAT HAS BEEN INITIALIZED:
  //for(int i=1; i<=numberOfItems; i++){Serial.println(ITEM[i]);}
}







  
void MENU::launchActivity(int& intToAdjust, float& floatToAdjust, bool& boolToAdjust){

    if(startTime==0){startTime = millis();} //Fixes a weird bug with remembering the prevButtonState;
    
    enum {_MIN, _MAX};
    
    //CHECK BUTTON STATUS NO SOONER THAN 500ms AFTER ACTIVITY START:
    if(millis()-startTime>500){_BUTTON.PRESS=_BUTTON.CHECK(_BUTTON.PIN, millis());}
    
    //BUTTON WAS CLICKED; MOVE CURSOR TO THE NEXT ITEM:
    if(_BUTTON.PRESS==_BUTTON.CLICK){
      _BUTTON.PRESS=_BUTTON.NO_ACTION;

      //LOOK THROUGH EACH ITEM:
      int i;
      for(i=1; i<=numberOfItems; i++){
        //IDENTIFY THE LOCATION OF THE CURSOR:
        if(ITEM[i].endsWith("*")){
          ITEM[i].remove(ITEM[i].length()-1);
          break;  //VALUE OF i IS THE LOCATION OF THE CURSOR;
        }  
      }
      //IDENTIFY LOCATION OF NEXT CURSORABLE ITEM:
      int j=i+1;
      while(true){
        if(j>numberOfItems){j=1;}
        //IF THE NEXT ITEM [j] HAS NO PURPOSE, LOOK AT THE NEXT [j]:
        if(PURPOSE[j]==NONE || PURPOSE[j]==NAMED_INT || PURPOSE[j]==NAMED_FLOAT){j++;}
        else{ITEM[j]+='*'; break;}  //VALUE OF j IS THE LOCATION OF THE NEXT CURSORABLE ITEM;
      }

      //DEBUG:
      for(int I=1; I<=numberOfItems; I++){Serial.println(ITEM[I]);}
      
    }
    
    //IF A BUTTON IS HELD, SELECT/TOGGLE THE ITEM:
    if(_BUTTON.PRESS==_BUTTON.HOLD){
      //howMany++;
      //Serial.print("howMany(MENU):"); Serial.println(howMany);
      _BUTTON.PRESS=_BUTTON.NO_ACTION;
    
      //LOOK AT EACH ITEM IN THE LIST:
      for(int i=1; i<=numberOfItems; i++){
        
        //IDENTIFY THE ITEM THAT HAS THE CURSOR:
        if(ITEM[i].endsWith("*")){


          /*************************************************************************************
            DECIDE ON AN ACTION TO TAKE WHEN ITEM IS SELECTED ACCORDING TO THE ITEM'S PURPOSE:
          /*************************************************************************************/

          if(PURPOSE[i]==INT_ADJUSTER){
            if(ITEM[i].startsWith("+")){
              intToAdjust += (ITEM[i].substring(1)).toInt();
              if(intToAdjust > INT_ADJUST[_MAX]){intToAdjust = INT_ADJUST[_MAX];} 
            }
            if(ITEM[i].startsWith("-")){
              intToAdjust -= (ITEM[i].substring(1)).toInt();
              if(intToAdjust < INT_ADJUST[_MIN]){intToAdjust = INT_ADJUST[_MIN];} 
            }            
            //UPDATE NAMED_INT VALUE:
            for(int i=1; i<=numberOfItems; i++){
              if(PURPOSE[i]==NAMED_INT){
                ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(intToAdjust);
              }
            }
          }

          if(PURPOSE[i]==FLOAT_ADJUSTER){
            if(ITEM[i].startsWith("+")){
              floatToAdjust += (ITEM[i].substring(1)).toInt();
              if(floatToAdjust > FLOAT_ADJUST[_MAX]){floatToAdjust = FLOAT_ADJUST[_MAX];} 
            }
            if(ITEM[i].startsWith("-")){
              floatToAdjust -= (ITEM[i].substring(1)).toInt();
              if(floatToAdjust < FLOAT_ADJUST[_MIN]){floatToAdjust = FLOAT_ADJUST[_MIN];} 
            }            
            //UPDATE NAMED_FLOAT VALUE:
            for(int i=1; i<=numberOfItems; i++){
              if(PURPOSE[i]==NAMED_FLOAT){
                ITEM[i] = ITEM[i].substring(0, ITEM[i].indexOf("=")+1) + round(floatToAdjust);
              }
            }
          }

          if(PURPOSE[i]==BOOL_TOGGLER){
            if(ITEM[i]==BOOL_DISPLAYED[0]+"*"){
              ITEM[i]=BOOL_DISPLAYED[1]+"*";
              boolToAdjust = 1;
            }
            else{
              ITEM[i]=BOOL_DISPLAYED[0]+"*";
              boolToAdjust = 0;
            }
          }

          if(PURPOSE[i]==ITEM_SWITCHER){
            for(int j=1; j<=numberOfItems; j++){
              if(PURPOSE[j]==ITEM_SWITCHER && ITEM[j].startsWith("*")){
                ITEM[j] = " "+ITEM[j].substring(1);
              }
            }
            ITEM[i].trim();
            ITEM[i] = "*"+ITEM[i];
            intToAdjust = i-1;
          }

          if(PURPOSE[i]==ACTIVITY_CHANGER){
            startTime = 0;
            CURRENT_PAGE = ACTIVITY[i];
          }
           
        }
      }

      //DEBUG:
      //for(int I=1; I<=numberOfItems; I++){Serial.println(ITEM[I]);}
      
    }
}
