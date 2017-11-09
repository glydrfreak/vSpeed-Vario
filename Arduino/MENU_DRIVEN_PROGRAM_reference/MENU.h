//#ifndef MENU_H
//#define MENU_H
////#include "OLED.h"
//#include <Arduino.h>
//
////PAGE NAMES:
//#define MAIN_ACTIVITY      0
//#define SETTINGS           1
//#define BEEP               2
//#define VOLUME             3
//#define THRESHOLD          4
//#define CLIMB              5
//#define SINK               6
//#define PITCH              7
//#define CLIMB_MAX          8
//#define CLIMB_MIN          9
//#define SINK_MAX          10
//#define SINK_MIN          11
//#define BLUETOOTH         12
//#define OLED              13
//#define CHART_SPEED       14
//#define USER              15
//#define EDIT_NAMES        16
//#define USER_1            17
//#define USER_2            18
//#define USER_3            19
//#define NO_NEXT           20
//
////ITEM TYPES:
//#define BLANK             0
//#define TITLE             1
//#define DISPLAYED_VALUE   2
//#define CLICKABLE         3
//#define EXPANDABLE        4
//#define CHECKABLE         5
//#define BACK              6
//#define EXIT              7
//
//  
//class MENU{
// public:
//
//
////    const int MAIN_ACTIVITY = 0;
////    const int SETTINGS = 1;
////    const int BEEP = 2;
////    const int VOLUME = 3;
////    const int THRESHOLD = 4;
////    const int CLIMB = 5;
////    const int SINK = 6;
////    const int PITCH = 7;
////    const int CLIMB_MAX = 8;
////    const int CLIMB_MIN = 9;
////    const int SINK_MAX = 10;
////    const int SINK_MIN = 11;
////    const int BLUETOOTH = 12;
////    const int OLED = 13;
////    const int CHART_SPEED = 14;
////    const int USER = 15;
////    const int EDIT_NAMES = 16;
////    const int USER_1 = 17;
////    const int USER_2 = 18;
////    const int USER_3 = 19;
////    const int NO_NEXT = 20;
//
//    /*int PREVIOUS_PAGE = MAIN_ACTIVITY;*/
//    int CURRENT_PAGE = MAIN_ACTIVITY;
//
//  
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
//
//
////    //ITEM TYPES:
////    const int BLANK             = 0;
////    const int TITLE             = 1;
////    const int DISPLAYED_VALUE   = 2;
////    const int CLICKABLE         = 3;
////    const int EXPANDABLE        = 4;
////    const int CHECKABLE         = 5;
////    const int BACK              = 6;
////    const int EXIT              = 7;
//
//    //OPEN UP A MENU_ACTIVITY WITH THE FOLLOWING PARAMETERS:
////    void MENU_ACTIVITY(
////      int INTENDED_PAGE, int PREVIOUS_PAGE,
////      int displayedValue, int displayedValueIncrement,
////      int displayedValueMin, int displayedValueMax, 
////      int itemOneType, String itemOne, int itemOneNextPage,
////      int itemTwoType, String itemTwo, int itemTwoNextPage,
////      int itemThreeType, String itemThree, int itemThreeNextPage,
////      int itemFourType, String itemFour, int itemFourNextPage,
////      int itemFiveType, String itemFive, int itemFiveNextPage
////    );
//
////PREVIOUS PAGES:
////    const int MAIN_ACTIVITY = 0;  //0
////    const int SETTINGS = 1;       //0
////    const int BEEP = 2;           //1
////    const int VOLUME = 3;         //2
////    const int THRESHOLD = 4;      //2
////    const int CLIMB = 5;          //4
////    const int SINK = 6;           //4
////    const int PITCH = 7;          //2
////    const int CLIMB_MAX = 8;      //7
////    const int CLIMB_MIN = 9;      //7
////    const int SINK_MAX = 10;      //7
////    const int SINK_MIN = 11;      //7
////    const int BLUETOOTH = 12;     //1
////    const int OLED = 13;          //1
////    const int CHART_SPEED = 14;   //13
////    const int USER = 15;          //1
////    const int EDIT_NAMES = 16;    //15
////    const int USER_1 = 17;        //16
////    const int USER_2 = 18;        //16
////    const int USER_3 = 19;        //16
//  
//};
//
//#endif
//
//

