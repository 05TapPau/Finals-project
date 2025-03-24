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
#include "FS.h"
#include <TFT_eSPI.h> // remove from libdeps
#include <FastLED.h>

// for testing only
unsigned long currentMillis = 0, previousMillis = 0;

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
int NavCounter = 0;                     //  Navigation counter for left right and up down

//  All TFT LCD erlated stuff
/*  TFT esp defines   >> Usersetup.h  for TrackBoard PCB
//#define TFT_MISO 19   //  Not used
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   33     // Chip select control pin
#define TFT_DC   25     // Data Command control pin
//#define TFT_RST  4    // Reset pin (could connect to RST pin)
#define TFT_RST  -1     // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TFT_BL   26  // LED back-light
#define TOUCH_CS 27     // Chip select pin (T_CS) of touch screen
*/
#define CALIBRATION_FILE "/calibrationData"

uint16_t x, y;
uint16_t calibrationData[5];
uint8_t calDataOK = 0;

TFT_eSPI tft = TFT_eSPI();

//  Fastled
// How many leds in your strip?
#define NUM_LEDS 1
#define DATA_PIN 13
CRGB leds[NUM_LEDS];

int hue = 0;

//  All GPS related stuff
//  Define the RX and TX pins for Serial 2
#define RXD2 17
#define TXD2 16
#define GPS_BAUD 9600
// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

// Start Serial 2 with the defined RX and TX pins and a baud rate of 9600

// A sample NMEA stream cuz GPS wont get a fix ffs
int simSeconds = 0;
const char *gpsStream =
    "$GPGGA,135135.130,4640.778,N,01109.212,E,1,12,1.0,0.0,M,0.0,M,,*6A\r\n"
    "$GPRMC,135135.130,A,4640.778,N,01109.212,E,,,030325,000.0,W*79\r\n";

//  All IMU related stuff
//  All SD related stuff

//  All usable Screens
void ScreenZero()
{
  tft.setTextSize(7);                                 //  Pixelsize of standart adafruit font 5x7 squares, 1 square 7x7 pixels spaceing inbetween characters is also 7 pixels
  tft.drawRoundRect(50, 50, 220, 100, 10, TFT_WHITE); //  Time
  tft.setCursor(73, 75);
  tft.println("12" /*(millis() / (10 * 60 * 60)) % 24 NEO6.time.hour()*/);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(143, 75);
  tft.println(":");
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  tft.setCursor(171, 75);
  tft.println("34" /*(millis() / (10 * 60)) % 60 NEO6.time.minute()*/);

  tft.drawRoundRect(50, 180, 220, 120, 10, TFT_WHITE); //  Date
  tft.setCursor(70, 205);
  tft.println("12" /*(millis() / (10 * 60 * 60)) % 24 NEO6.time.hour()*/);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(139, 205);
  tft.println(".");
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  tft.setCursor(171, 205);
  tft.println("34" /*(millis() / (10 * 60)) % 60 NEO6.time.minute()*/);
  tft.setTextSize(4);
  tft.setCursor(114, 265);
  tft.println("2025");

  tft.setTextSize(7);
  tft.drawRoundRect(50, 330, 220, 100, 10, TFT_WHITE); //  Temp

  /*
  tft.setCursor(120, 10);
  tft.println((millis() / (10 * 60 * 60)) % 24 /*NEO6.time.hour());
  tft.print("  ");
  tft.setCursor(120, 70);
  tft.println((millis() / (10 * 60)) % 60 /*NEO6.time.minute());
  tft.setCursor(120, 130);
  tft.println((millis() / 10) % 60 /*NEO6.time.second());

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
void ScreenOne()
{
  tft.setTextSize(4);

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
void ScreenTwo()
{
  tft.setTextSize(4);

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
void Screenthree()
{
  tft.setTextSize(4);

  tft.setCursor(10, 10);
  tft.println("fourth");
  tft.setCursor(10, 110);
  tft.println("screen");
}
void ScreenFour()
{
  tft.setTextSize(4);

  tft.setCursor(10, 10);
  tft.println("fifth");
  tft.setCursor(10, 110);
  tft.println("screen");
}
void ScreenFive()
{
  tft.setTextSize(4);

  tft.setCursor(10, 10);
  tft.println("sixth");
  tft.setCursor(10, 110);
  tft.println("screen");
}
void ScreenSix()
{
  tft.setTextSize(4);

  tft.setCursor(10, 10);
  tft.println("seventh");
  tft.setCursor(10, 110);
  tft.println("screen");
}
void ScreenSeven()
{
  tft.setTextSize(4);

  tft.setCursor(10, 10);
  tft.println("eighth");
  tft.setCursor(10, 110);
  tft.println("screen");
}
void Screeneight()
{
  tft.setTextSize(4);

  tft.setCursor(10, 10);
  tft.println("ninth");
  tft.setCursor(10, 110);
  tft.println("screen");
}

//  Screen Navigation
void ScreenNav()
{
  // screennavigation
  if (gen_edge_det)
  {
    tft.fillScreen(TFT_BLACK);
    gen_edge_det = false;
  }

  switch (NavCounter)
  {
  case 0:
    ScreenZero();
    break;
  case 1:
    ScreenOne();
    break;
  case 2:
    ScreenTwo();
    break;
  case 3:
    Screenthree();
    break;
  case 4:
    ScreenFour();
    break;
  case 5:
    ScreenFive();
    break;
  case 6:
    ScreenSix();
    break;
  case 7:
    ScreenSeven();
    break;
  case 8:
    Screeneight();
    break;
  default:
    break;
  }
}

//  Handle Buttons/Touchscreen inputs
void updateCounter(int increment)
{
  NavCounter = (NavCounter + increment + 9) % 9; // Ensure counter stays within 0-9 range
  Serial.print("Counter: ");
  Serial.println(NavCounter);
}

//  check for Button presses
void checkButtons() // Debounce handled by Hardware 100mF across Buttons
{
  if (digitalRead(INPUT0) == 0 and digitalRead(INPUT0) != PrevState[0])
  {
    Serial.println("I0 pressed");
    edgedetected[0] = true;
    gen_edge_det = 1;
    updateCounter(3); // Add 3
  }
  else if (digitalRead(INPUT1) == 0 and digitalRead(INPUT1) != PrevState[1])
  {
    Serial.println("I1 pressed");
    edgedetected[1] = true;
    gen_edge_det = 1;

    updateCounter(1); // Add 1
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
    updateCounter(-3); // Subtract 3
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

//  check for Touchscreen inputs
void HandleTouchscreen()
{
  if (tft.getTouch(&x, &y))
  {
    if (x > 106 and x < 214 and y < 330 and y > 180 and !edgedetected[2]) //  center
    {
      Serial.println("I2 pressed");
      edgedetected[2] = true;
      gen_edge_det = 1;
    }
    if (x < 106 and x > 0 and y < 330 and y > 180 and !edgedetected[4]) //  Up
    {
      Serial.println("I4 pressed");
      edgedetected[4] = true;
      gen_edge_det = 1;

      updateCounter(3); // Add 3
    }
    if (x < 320 and x > 214 and y < 330 and y > 180 and !edgedetected[1]) //  Down
    {
      Serial.println("I1 pressed");
      edgedetected[1] = true;
      gen_edge_det = 1;

      updateCounter(-3); // Subtract 3
    }
    if (y < 480 and y > 330 and x < 214 and x > 106 and !edgedetected[0]) //  Left
    {
      Serial.println("I0 pressed");
      edgedetected[0] = true;
      gen_edge_det = 1;

      updateCounter(1); // Add 1
    }
    if (y < 180 and y > 30 and x < 214 and x > 106 and !edgedetected[3]) //  Right
    {
      Serial.println("I3 pressed");
      edgedetected[3] = true;
      gen_edge_det = 1;

      updateCounter(-1); // Subtract 1
    }
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
}

//  WS2812 Ledshenanigans
void Ledshenanigans()
{
  leds[0] = CHSV(hue++, 255, 255);
  FastLED.show();
  if (hue == 255)
    hue = 0;
}

/*
//  Handle NEO6M GPS module
-  Get GPS fix
-  Get NMEA data
-  Parse NMEA data
-  Get GPS sattelite count
-  Get GPS location
-  Get GPS time
-  Get GPS altitude
-  Get GPS speed
-  Get GPS course
-  give data to screen
  -  0 Time and Date and temp
  -  1 Lon Lat Alt Speed
  -  2 Sattelite count and heading/course
  -  3 Big Speedometer
  -  4 List of sattelites else maybe a matrix for WS2812 8x8 controll?/ button toactivate g sensing activation of WS2812
  -  6 Big Compass no sensor  would be cool tho


//  Handle IMU MPU6050
-  Get IMU data
-  Get IMU gyro data
-  Get IMU accel data
-  Get IMU temp data
-  give data to screen
  -  0 Time and Date and temp
  -  5 rotation of the device and accelleration

//  Handle SD
-  SD will be used to store the GPX data
-  did a tracking start? check the touchscreen/button
-  did a tracking stop? check the touchscreen/button
-  check if SD card is present
-  if not present give error to screen
-  if present create a GPX-file
-  write GPS-parsed data to file in gpx format

-  give data to screen
  -  7 idk honestly SD card info? tracking info and buttons to start/stop tracking (center button/touchfield)
  -  8 error inf screen GPS not found/no fix or SD card not found, maybe return to screen 0 after 5 seconds and return here again if no fix is found every minute or so
*/

//  Just a blinky thing
//  Mainly for debuging and sanity checks
void StillAlive()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= 100)
  {
    previousMillis = currentMillis;
    digitalWrite(2, !digitalRead(2));




    
    while (gpsSerial.available() > 0)
    {
      // get the byte data from the GPS
      char gpsData = gpsSerial.read();
      Serial.print(gpsData);
    }
  }
}

void setup()
{
  // Pins used for Buttons, alive Led and backlight
  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(2, OUTPUT);
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);

  //  Start serial comunication for debuging
  Serial.begin(115200);

  //  Setup ws2812 led
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(2);

  //  Start tft
  tft.init();
  tft.fillScreen(TFT_BLACK);

  // calibrate touch
  tft.setRotation(3);
  tft.fillScreen((0xFFFF));
  tft.setCursor(20, 0, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.println("calibration run");
  // check file system
  if (!SPIFFS.begin())
  {
    Serial.println("formatting file system");

    SPIFFS.format();
    SPIFFS.begin();
  }
  // check if calibration file exists
  if (SPIFFS.exists(CALIBRATION_FILE))
  {
    File f = SPIFFS.open(CALIBRATION_FILE, "r");
    if (f)
    {
      if (f.readBytes((char *)calibrationData, 14) == 14)
        calDataOK = 1;
      f.close();
    }
  }
  if (calDataOK)
  {
    // calibration data valid
    tft.setTouch(calibrationData);
  }
  else
  {
    // data not valid. recalibrate
    tft.calibrateTouch(calibrationData, TFT_WHITE, TFT_RED, 15);
    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f)
    {
      f.write((const unsigned char *)calibrationData, 14);
      f.close();
    }
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  tft.setRotation(2);
  tft.setTextFont(1); // font 2 kinda nice

  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  if (gpsSerial.available())
  {
    Serial.print("Serial 2 at ");
    Serial.print(GPS_BAUD);
    Serial.print(" baud"); // => Dev_Debug(ReportGPSSetup);
  }
}

void loop()
{

  // checkButtons(); //  Buttons work
  HandleTouchscreen();
  ScreenNav();
  // Ledshenanigans();

  StillAlive(); // just blink build in led 1Hz
}