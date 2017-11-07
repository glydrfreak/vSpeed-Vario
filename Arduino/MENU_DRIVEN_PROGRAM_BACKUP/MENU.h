#ifndef MENU_H
#define MENU_H

#include <Arduino.h>


  
class MENU{
 public:

    const int MAIN_ACTIVITY = 0;
    const int SETTINGS = 1;
    const int BEEP = 2;
    const int VOLUME = 3;
    const int THRESHOLD = 4;
    const int CLIMB = 5;
    const int SINK = 6;
    const int PITCH = 7;
    const int CLIMB_MAX = 8;
    const int CLIMB_MIN = 9;
    const int SINK_MAX = 10;
    const int SINK_MIN = 11;
    const int BLUETOOTH = 12;
    const int OLED = 13;
    const int CHART_SPEED = 14;
    const int USER = 15;
    const int EDIT_NAMES = 16;
    const int USER_1 = 17;
    const int USER_2 = 18;
    const int USER_3 = 19;


  //String theMENU[19][5] = {};
  int CURRENT_PAGE = 0;
  
  //INDICATE HIGHLIGHTED ITEM WITH ...'*'
  //void highlight(int pg, int itm){
    //for(int i=0; i<6; i++){theMENU[pg][i].replace('*',' ');} 
    //theMENU[pg][itm]+='*';
  //} 
  
  /*//GO TO A SPECIFIED PAGE AND HIGHLIGHT THE FIRST OPTION BEGINNING ON THE SECOND ROW
  void goTo(int pg){CURRENT_PAGE=pg;} */
  
  //INDICATE SELECTION OF HIGHLIGHTED ITEM WITH '->'...
  //void select(int pg, int itm){theMENU[pg][itm]="->"+theMENU[pg][itm];} 
  
  //TELL THE PROGRAM YOU ARE INTERESTED IN CHANGING A SPECIFIED MENU ITEM
  //void toggle(){} 

  
};

#endif



