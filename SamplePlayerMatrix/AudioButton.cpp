/* 
 * AudioButtons.cpp - Library to link Audiofiles 
 * to Buttons
 */
#include"AudioButton.h"

AudioButton::AudioButton(int nr){
    this->_nr=nr;
}

void AudioButton::setTrack(char* tracki){
    this->track=tracki;
}

char* AudioButton::getTrack()const{
    return(this->track);  
}
