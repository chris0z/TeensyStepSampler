#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "AudioButton.h"
#include<Keypad.h>

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

AudioButton btn1(1);
AudioButton btn2(2);
AudioButton btn3(3);
AudioButton btn4(4);
AudioButton btn5(5);
AudioButton btn6(6);
AudioButton btn7(7);
AudioButton btn8(8);

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlaySdWav     playWav1;
AudioPlaySdWav     playWav2;
AudioPlaySdWav     playWav3;
AudioPlaySdWav     playWav4;
AudioPlaySdWav     playWav5;
AudioPlaySdWav     playWav6;
AudioPlaySdWav     playWav7;
AudioPlaySdWav     playWav8;

AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
AudioMixer4        mix3;    // tandem to combine 6 audio sources
AudioOutputI2S     headphones;
AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
AudioConnection patchCord1(playWav1,0,mix1,0);
AudioConnection patchCord2(playWav2,0,mix1,1);
AudioConnection patchCord3(playWav3,0,mix1,2);
AudioConnection patchCord4(playWav4,0,mix1,3);
AudioConnection sum1(mix1, 0, mix2, 0);
AudioConnection patchCord5(playWav5,0,mix2,1);
AudioConnection patchCord6(playWav6,0,mix2,2);
AudioConnection patchCord7(playWav7,0,mix2,3);
AudioConnection sum2(mix2, 0, mix3, 0);
AudioConnection patchCord8(playWav8,0,mix3,1);

AudioConnection c8(mix3, 0, headphones, 0);
AudioConnection c9(mix3, 0, headphones, 1);
AudioConnection c10(mix3, 0, dac, 0);

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
  mix2.gain(3, 0.4);
  mix3.gain(1, 0.4);
  
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
btn1.setTrack((char*)"1.WAV");
char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
  // When the buttons are pressed, just start a sound playing.
  // The audio library will play each sound through the mixers
  // so any combination can play simultaneously.
  //
  
  if (key=='1') {
   //playFile
   playWav1.play("1.WAV");
  }
  if (key=='2') {
   //playFile
   playWav2.play("2.WAV");
  }
  if (key=='3') {
   //playFile
   playWav3.play("3.WAV");
  }

  if (key=='4') {
   //playFile
   playWav1.play("4.WAV");
  }
  if (key=='5') {
   //playFile
   playWav2.play("5.WAV");
  }
  if (key=='6') {
   //playFile
   playWav3.play("6.WAV");
  }
  
  if (key=='7') {
   //playFile
   playWav3.play("7.WAV");
  }
  
  if (key=='8') {
   //playFile
   playWav3.play("8.WAV");
  }
}

