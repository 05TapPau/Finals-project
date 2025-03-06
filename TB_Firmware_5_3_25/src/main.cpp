//  Firmware for a finals project at the school TFO-Meran 2025
//  Project Name:
//  -TrackBoard
//  Usecase:
//  -Tracking GPS-data and Gyroscopic data writing it to a SD card and displaying it to a 3.5" TFT LCD
//  -Userinputs via 5Way switch to navigate Menues and select actions
//  
//  This code is opensource and free to use for personal use aswell as all other files found within this repository
//  If used for instituional (say school Uni etc.) still free to use but mark copyed elements as such/as requiered
//  If used for comercial free to use if you can make money from it good for u but reconsider hardwarecoices (there bad for com use its a prototype)
//  If you found this code helpfull and or have any questions contact me i would be interested to se what others do with this prototype
//  Have fun

//  included Librries
#include <Arduino.h>

//  Input Buttons (5Way)  No use if used with touch
#define INPUT0 34 //Right
#define INPUT1 35 //Up
#define INPUT2 32 //Push
#define INPUT3 36 //Left
#define INPUT4 39 //Down

bool CurrState[5]={0,0,0,0,0};
bool PrevState[5]={0,0,0,0,0}; //used to check for falling edges (Buttons pullups)

void checkButtons()// Debounce handled by Hardware 100mF across Buttons
{
  if (digitalRead(INPUT0) == 0 and digitalRead(INPUT0) != PrevState[0])
  {
    Serial.println("I0 pressed");
  }
  PrevState[0] = digitalRead(INPUT0);
  if (digitalRead(INPUT1) == 0 and digitalRead(INPUT1) != PrevState[1])
  {
    Serial.println("I1 pressed");
  }
  PrevState[1] = digitalRead(INPUT1);
  if (digitalRead(INPUT2) == 0 and digitalRead(INPUT2) != PrevState[2])
  {
    Serial.println("I2 pressed");
  }
  PrevState[2] = digitalRead(INPUT2);
  if (digitalRead(INPUT3) == 0 and digitalRead(INPUT3) != PrevState[3])
  {
    Serial.println("I3 pressed");
  }
  PrevState[3] = digitalRead(INPUT3);
  if (digitalRead(INPUT4) == 0 and digitalRead(INPUT4) != PrevState[4])
  {
    Serial.println("I4 pressed");
  }
  PrevState[4] = digitalRead(INPUT4);
}

void setup() {
  Serial.begin(115200);

  pinMode(INPUT0,INPUT);
  pinMode(INPUT1,INPUT);
  pinMode(INPUT2,INPUT);
  pinMode(INPUT3,INPUT);
  pinMode(INPUT4,INPUT);
}

void loop() {
  checkButtons();
}