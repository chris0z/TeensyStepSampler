#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <Keypad.h>

// WAV files converted to code by wav2sketch
#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
  };

byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 5, 4}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

class AudioButton{
  public:
    AudioButton(int nr);
    void setTrack(char* track);
    string getTrack();
  private:
    int _nr;
};
AudioButton::AudioButton(int nr){
 _nr=nr
}
void AudioButton::setTrack(char* track){

}
voidAudioButton::getTrack(char* track){
  
}


// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlaySdWav     playWav1;
AudioPlaySdWav     playWav2;
AudioPlaySdWav     playWav3;
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;  // six memory players, so we can play
AudioPlayMemory    sound2;  // all six sounds simultaneously
AudioPlayMemory    sound3;
AudioPlayMemory    sound4;
AudioPlayMemory    sound5;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
AudioMixer4        mix3;    // tandem to combine 6 audio sources
AudioOutputI2S     headphones;
AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2
AudioConnection c6(sound4, 0, mix2, 1);
AudioConnection c7(sound5, 0, mix2, 2);
AudioConnection patchCord1(playWav1,0,mix2,3);
AudioConnection c11(mix2, 0, mix3, 0);
AudioConnection patchCord2(playWav2,0,mix3,1);
AudioConnection patchCord3(playWav3,0,mix3,2);
AudioConnection c8(mix3, 0, headphones, 0);
AudioConnection c9(mix3, 0, headphones, 1);
AudioConnection c10(mix3, 0, dac, 0);

/*
AudioConnection patchCord2(playWav1, 1, mix3, 1);
AudioConnection patchCord11(playWav1,0,mix3,0);
AudioConnection patchCord12(playWav1, 1, mix3, 1);
*/
// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;
// Use these with the audio adaptor board
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
void setup() {
//keypad.addEventListener(keypadEvent); //add an event listener for this keypad

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.50);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.4);

    SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}
void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  while (playWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  }
}

void loop() {
char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
  // When the buttons are pressed, just start a sound playing.
  // The audio library will play each sound through the mixers
  // so any combination can play simultaneously.
  //
  
  if (key=='1') {
    sound0.play(AudioSampleSnare);
  }
  if (key=='2') {
    sound1.play(AudioSampleTomtom);
  }
  if (key=='3') {
    sound2.play(AudioSampleHihat);
  }
  if (key=='4') {
    sound3.play(AudioSampleKick);
  }
  if (key=='5') {
    // comment this line to work with Teensy 3.0.
    // the Gong sound is very long, too much for 3.0's memory
    sound4.play(AudioSampleGong);
  }
  if (key=='6') {
    sound5.play(AudioSampleCashregister);
  }
  if (key=='7') {
   //playFile
   playWav1.play("7.WAV");
  }
  if (key=='8') {
   //playFile
   playWav2.play("8.WAV");
  }
  if (key=='9') {
   //playFile
   playWav3.play("9.WAV");
  }
}

