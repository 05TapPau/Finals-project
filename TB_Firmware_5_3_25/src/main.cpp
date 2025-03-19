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

// Testing only
unsigned long currentMillis = 0, previousMillis = 0;

//  included Librries
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <FastLED.h>

// All Button related stuff
//   Input Buttons (5Way)  No use if used with touch
#define INPUT0 34 // Right
#define INPUT1 35 // Up
#define INPUT2 32 // Push
#define INPUT3 36 // Left
#define INPUT4 39 // Down

//  Flags for Button presses (and or touch recogintion future?)
bool edgedetected[5] = {0, 0, 0, 0, 0}; //  flags to recognize if there was a falling edge (only for 1 cycle)
bool PrevState[5] = {0, 0, 0, 0, 0};    // used to check for falling edges (Buttons pullups)
bool gen_edge_det = 0;                  // gets set if there was a edge detected no matter what pin
int ScreenNavCounter = 1;

void checkButtons() // Debounce handled by Hardware 100mF across Buttons
{
  if (digitalRead(INPUT0) == 0 and digitalRead(INPUT0) != PrevState[0])
  {
    Serial.println("I0 pressed");
    edgedetected[0] = true;
    gen_edge_det = 1;
  }
  else if (digitalRead(INPUT1) == 0 and digitalRead(INPUT1) != PrevState[1])
  {
    Serial.println("I1 pressed");
    edgedetected[1] = true;
    gen_edge_det = 1;

    if (ScreenNavCounter < 2)
    {
      ScreenNavCounter++;
    }
    else
      ScreenNavCounter = 0;
  }
  else if (digitalRead(INPUT2) == 0 and digitalRead(INPUT2) != PrevState[2])
  {
    Serial.println("I2 pressed");
    edgedetected[2] = true;
    gen_edge_det = 1;
  }
  else if (digitalRead(INPUT3) == 0 and digitalRead(INPUT3) != PrevState[3])
  {
    Serial.println("I3 pressed");
    edgedetected[3] = true;
    gen_edge_det = 1;
  }
  else if (digitalRead(INPUT4) == 0 and digitalRead(INPUT4) != PrevState[4])
  {
    Serial.println("I4 pressed");
    edgedetected[4] = true;
    gen_edge_det = 1;

    if (ScreenNavCounter > 0)
    {
      ScreenNavCounter--;
    }
    else
      ScreenNavCounter = 2;
  }
  else
  {
    edgedetected[0] = false;
    edgedetected[1] = false;
    edgedetected[2] = false;
    edgedetected[3] = false;
    edgedetected[4] = false;

    gen_edge_det = 0;
  }

  PrevState[0] = digitalRead(INPUT0);
  PrevState[1] = digitalRead(INPUT1);
  PrevState[2] = digitalRead(INPUT2);
  PrevState[3] = digitalRead(INPUT3);
  PrevState[4] = digitalRead(INPUT4);
}

//  All TFT LCD erlated stuff
/*  TFT esp defines   >> Usersetup.h  for my TrackBoard PCB

//#define TFT_MISO 19   //  Not used
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   33     // Chip select control pin
#define TFT_DC   25     // Data Command control pin
//#define TFT_RST  4    // Reset pin (could connect to RST pin)
#define TFT_RST  -1     // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TFT_BL   26  // LED back-light
*/

TFT_eSPI tft = TFT_eSPI();

void Homescreen()
{
  tft.setRotation(2);
  // tft.setTextFont(1); // font 2 kinda nice

  tft.setCursor(120, 10);
  tft.println((millis() / (10 * 60 * 60)) % 24 /*NEO6.time.hour()*/);
  tft.print("  ");
  tft.setCursor(120, 70);
  tft.println((millis() / (10 * 60)) % 60 /*NEO6.time.minute()*/);
  tft.setCursor(120, 130);
  tft.println((millis() / 10) % 60 /*NEO6.time.second()*/);

  /*
tft.setCursor(30, 30);
tft.println("Date: ");
tft.setCursor(30, 90);
tft.print(NEO6.date.day());
tft.print(".");
tft.print(NEO6.date.month());
tft.print(".");
tft.print(NEO6.date.year());
*/
}

void leftscreen()
{
  tft.setRotation(2);
  tft.setTextSize(5);

  tft.setCursor(10, 10);
  tft.println("Lon: ");
  tft.setCursor(10, 60);
  // tft.print(NEO6.location.lng(),3);
  tft.setCursor(10, 110);
  tft.println("Lat: ");
  tft.setCursor(10, 160);
  // tft.print(NEO6.location.lat(),3);
  tft.setCursor(10, 210);
  tft.println("Alt: ");
  tft.setCursor(10, 260);
  // tft.print(NEO6.altitude.meters(),3);
  tft.setCursor(10, 310);
  tft.println("Speed: ");
  tft.setCursor(40, 360);
  // tft.print(NEO6.speed.kmph());
  tft.print(" km/h");
}

void rightscreen()
{
  tft.setRotation(2);
  tft.setTextSize(5);

  tft.setCursor(10, 10);
  tft.println("third");
  tft.setCursor(10, 60);
  // tft.print(NEO6.location.lng(),3);
  tft.setCursor(10, 110);
  tft.println("screen");
  tft.setCursor(10, 160);
  // tft.print(NEO6.location.lat(),3);
  tft.setCursor(10, 210);
  tft.println("idk");
  tft.setCursor(10, 260);
  // tft.print(NEO6.altitude.meters(),3);
}

void NavFrames()
{
  // screennavigation
  if (gen_edge_det)
  {
    tft.fillScreen(0x0000);
  }
  switch (ScreenNavCounter)
  {
  case 0:
    leftscreen();
    break;
  case 1:
    Homescreen();
    break;
  case 2:
    rightscreen();
    break;
  default:
    break;
  }
}

//  Fastled
// How many leds in your strip?
#define NUM_LEDS 1
#define DATA_PIN 13
CRGB leds[NUM_LEDS];

int hue = 0;

void Ledshenanigans()
{
  leds[0] = CHSV(hue++, 255, 255);
  FastLED.show();
  if (hue == 255)
    hue = 0;
}

//  All GPS related stuff
// A sample NMEA stream cuz GPS wont get a fix ffs
int simSeconds = 0;
const char *gpsStream =
    "$GPGGA,135135.130,4640.778,N,01109.212,E,1,12,1.0,0.0,M,0.0,M,,*6A\r\n"
    "$GPRMC,135135.130,A,4640.778,N,01109.212,E,,,030325,000.0,W*79\r\n";

//  All IMU related stuff
//  All SD related stuff

//  Mainly for debuging and sanity checks
void StillAlive()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= 100)
  {
    previousMillis = currentMillis;
    digitalWrite(2, !digitalRead(2));
  }
}

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  tft.init();
  tft.fillScreen(0x0000);
  tft.setTextSize(7);
  tft.setTextColor(0xFFFF, 0x0000, true);

  Serial.begin(115200);

  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(2, OUTPUT);

  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
}

void loop()
{
  checkButtons(); //  Buttons work
  NavFrames();
  Ledshenanigans();

  StillAlive(); // just blink build in led 1Hz
}