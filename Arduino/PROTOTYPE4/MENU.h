#ifndef MENU_H
#define MENU_H
#include <Arduino.h>


 //TODO-- UPDATE THIS EXAMPLE TABLE:
 
//    //MORE THAN 9 CHARS== NEWLINE     //"---------";
//    //MORE THAN 6 ITEMS== OFF SCREEN  //"---------";
//    //ITEM3                           //"---------";
//    //ITEM4                           //"---------";
//    //ITEM5                           //"---------";
//    //ITEM6                           //"---------";
//
//    
//    //ITEMTYPE:_______________________//EXAMPLE:______________//onClick:____________________//onHold:_______________________________________//FUNCTION:_______
//    //itemType[0]=//blank             //"         ";          //N/A                         //N/A                                           //
//    //itemType[1]=//title             //"SETTINGS ";          //N/A                         //N/A                                           //
//    //itemType[2]=//displayedValue    //"VOL=75   ";          //N/A                         //N/A                                           //
//    //itemType[3]=//clickable         //"  +25%*  ";          //HIGHLIGHT NEXT ITEM         //UPDATES "displayedValue"                      //
//    //itemType[4]=//expandable        //"  >VOL*  ";          //HIGHLIGHT NEXT ITEM         //GO TO SPECIFIED MENU                          //
//    //itemType[5]=//checkable         //"->ON"*   ";          //HIGHLIGHT NEXT ITEM         //MARK CURRENT ITEM, UN-MARK SPECIFIED OTHER    //
//    //itemType[6]=//back              //"<-*   -> ";          //HIGHLIGHT NEXT ITEM         //GO TO PREVIOUS MENU                           //
//    //itemType[7]=//exit              //"<-    ->*";          //HIGHLIGHT NEXT ITEM         //GO TO MAIN_ACTIVITY                           //


class MENU{
  private:
  int howMany=0;
  int startTime = 0;
  
  public:

  //POSSIBLE ITEM PURPOSES:
  enum {NONE, NAMED_INT, NAMED_FLOAT, INT_ADJUSTER, ITEM_SWITCHER, FLOAT_ADJUSTER, BOOL_TOGGLER, ACTIVITY_CHANGER};
  
  //FILL THESE ARRAYS FOR ORGANIZATION:
  static const int numberOfItems           = 7;  //ITEM_1, ITEM_2, ITEM_3, ITEM_4, ITEM5, GO_TO_PREVIOUS_PAGE, GO_TO_MAIN_ACTIVITY;
  String ITEM[numberOfItems+1]            = {};  //ITEM NAME AND FORMAT TO DISPLAY
  int PURPOSE[numberOfItems+1]            = {};  //POSSIBLE PURPOSES ON ITEM SELECTION
  int ACTIVITY[numberOfItems+1]           = {};  //POSSIBLE NEXT PAGES ON ITEM SELECTION
  int INT_ADJUST[numberOfItems+1]         = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF AN INTEGER VARIABLE
  int FLOAT_ADJUST[numberOfItems+1]       = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF A FLOATING POINT VARIABLE
  String BOOL_DISPLAYED[numberOfItems+1]  = {};  //PARAMETERS FOR ADJUSTING THE VALUE OF A BOOLEAN VARIABLE: "[ON]" OR "[OFF]"
  
  int CURRENT_PAGE = 0;
  
  void menuItem(String item1, String item2, String item3, String item4, String item5, String item6, String item7);
  void itemPurpose(int purpose1, int purpose2, int purpose3, int purpose4, int purpose5, int purpose6, int purpose7);
  void ifSelectedGoTo(int activity1, int activity2, int activity3, int activity4, int activity5, int activity6, int activity7);
  void integerAdjust(int integerMin, int integerMax);
  void floatAdjust(float floatMin, float floatMax);
  void booleanToggle(String displayForTrue, String displayForFalse);
  void initializeActivity(int& integerToAdjust, float& floatToAdjust, bool& boolToAdjust);
  void launchActivity(int& intToAdjust, float& floatToAdjust, bool& boolToAdjust);
  
  int NO_INT = 0;
  float NO_FLOAT = 0;
  bool NO_BOOL = 0;
  
};


#endif


