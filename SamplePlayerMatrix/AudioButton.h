/*
 * AudioButton.h Library to Link Audiofiles to Buttons
 */
#ifndef AudioButton_h
#define AudioButton_h

#include<SD.h>
//#include"Arduino.h"

class AudioButton{
 
  public:
    AudioButton(int nr);
    void setTrack(char* tracki);
    char*getTrack()const; 
  private:
    int _nr;
    char* track;
};
#endif

