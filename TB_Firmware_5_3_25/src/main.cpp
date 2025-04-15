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
#include "Wire.h"
#include "SD.h"
#include "TinyGPS++.h"

//  Logo
//  #include "TBLOGO.c" changed to SPIFFs

// All Button related stuff
//   Input Buttons (5Way)  No use if used with touch
#define INPUT0 34 // Right
#define INPUT1 35 // Up
#define INPUT2 32 // Push
#define INPUT3 36 // Left
#define INPUT4 39 // Down

#define TFT_CS 33
#define TOUCH_CS 27
#define SD_CS 14

// TFT Touch calibration
#define CALIBRATION_FILE "/calibrationData"

//  Timing
unsigned long currentMillis = 0, previousMillis = 0, currentMillis2 = 0, previousMillis2 = 0, cBreakMillis3 = 0, prvBreakMillis3 = 0; //  millis() timer for all the things
bool SecondsToggle = 0;   //  Toggle for seconds to update the screen every second

//  Flags for Button presses (and or touch recogintion future?)
bool edgedetected[5] = {0, 0, 0, 0, 0}; //  flags to recognize if there was a falling edge (only for 1 cycle)
bool PrevState[5] = {0, 0, 0, 0, 0};    // used to check for falling edges (Buttons pullups)
bool gen_edge_det = 0;                  // gets set if there was a edge detected no matter what pin
int NavCounter = 0;                     //  Navigation counter for left right and up down

//  All TFT LCD erlated stuff
uint16_t x, y;
uint16_t calibrationData[5];
uint8_t calDataOK = 0;
TFT_eSPI tft = TFT_eSPI();

//  Fastled
#define NUM_LEDS 17 //  1 onboard 16 external(2x8)
#define DATA_PIN 13
CRGB leds[NUM_LEDS];

//  All GPS related stuff
HardwareSerial gpsSerial(2);
TinyGPSPlus NEO6;
String year, month, day, hour, minute, second;
bool Tracking = false; //  Tracking flag for SD card writing

//  Define the RX and TX pins for Serial 2
#define RXD2 17
#define TXD2 16
#define GPS_BAUD 9600

// A sample NMEA stream cuz GPS wont get a fix ffs
int simSeconds = 0;
const char *gpsStream =
    "$GPGGA,142736.660,4640.778,N,01109.212,E,1,12,1.0,0.0,M,0.0,M,,*6D\r\n"
    "$GPRMC,142736.660,A,4640.778,N,01109.212,E,,,150425,000.0,W*7E\r\n";

//  GPS related functions
void fakenema()
{
  // get fake nema info for gps cuz gps wont get a fix
  while (*gpsStream)
    NEO6.encode(*gpsStream++);
  delay(100);
}
void realNMEA(){
  // get real nema info for gps
  while (gpsSerial.available())
  {
    NEO6.encode(gpsSerial.read());
  }
}

//  All IMU related stuff
float AccX, AccY, AccZ, AngleRoll, AnglePitch, AngleYaw, tempC;

//  All SD related stuff
#define SD_CS 14             // Chip select pin for SD card module
#define LoggingInterval 3000 // Interval for logging data in milliseconds 3 sec int
File root;
File dataFile;
String fileID;

//  All usable Screens
void ScreenZero()
{
  //  Screen 0refresh screen every second cuz no important data
  currentMillis = millis();
  if (currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;

    tft.setCursor(144, 75);
    if (SecondsToggle)
    {
      tft.println(":");
      SecondsToggle = false;
    }
    else
    {
      SecondsToggle = true;
      tft.println(" ");
    }
    tft.setTextSize(7);                                 //  Pixelsize of standart adafruit font 5x7 squares, 1 square 7x7 pixels spaceing inbetween characters is also 7 pixels
    tft.drawRoundRect(50, 50, 220, 100, 10, TFT_WHITE); //  Time
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.setCursor(65, 75);
    tft.println(NEO6.time.hour()); //  (millis() / (10 * 60 * 60)) % 24 NEO6.time.hour()
    tft.setCursor(179, 75);
    tft.println(NEO6.time.minute()); //  (millis() / (10 * 60)) % 60 NEO6.time.minute()

    tft.drawRoundRect(50, 180, 220, 120, 10, TFT_WHITE); //  Date
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(70, 205);
    tft.println(NEO6.date.day()); //   (millis() / (10 * 60 * 60)) % 24 NEO6.time.hour()
    tft.setCursor(150, 225);
    tft.setTextSize(4);
    tft.println(".");
    tft.setTextSize(7);
    tft.setCursor(171, 205);
    tft.println(NEO6.date.month()); //   (millis() / (10 * 60)) % 60 NEO6.time.minute()
    tft.setTextSize(4);
    tft.setCursor(114, 265);
    tft.println(NEO6.date.year());

    tft.setTextSize(6);
    tft.drawRoundRect(50, 330, 220, 100, 10, TFT_WHITE); //  Temp
    tft.setTextColor(TFT_WHITE, TFT_BLACK, false);
    tft.setCursor(75, 355);
    tft.println(tempC);
  }

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
  // prepare screen
  tft.setTextSize(4);

  //  draw longitude, latitude and altitude
  tft.drawRoundRect(10, 5, 300, 110, 15, TFT_WHITE);
  tft.setCursor(30, 25);
  tft.println("Lon: ");
  tft.setCursor(30, 70);
  tft.println(NEO6.location.lng(), 5); //  5 decimals, accuracy of 1.1m, 4 11m, 3 111m
  // tft.print("");

  tft.drawRoundRect(10, 125, 300, 110, 15, TFT_WHITE);
  tft.setCursor(30, 145);
  tft.println("Lat: ");
  tft.setCursor(30, 190);
  tft.println(NEO6.location.lat(), 5);
  // tft.print("");

  tft.drawRoundRect(10, 245, 300, 110, 15, TFT_WHITE);
  tft.setCursor(30, 265);
  tft.println("Alt: ");
  tft.setCursor(30, 310);
  tft.print(NEO6.altitude.meters(), 1);
  // tft.print("300.0");
  tft.print(" m");

  tft.drawRoundRect(10, 365, 300, 110, 15, TFT_WHITE);
  tft.setCursor(30, 385);
  tft.println("Heading: ");
  tft.setCursor(30, 430);
  tft.print(NEO6.course.deg(), 2);
  tft.print("deg");
}
void ScreenTwo()
{
  tft.setTextSize(4);
  tft.drawRoundRect(50, 50, 220, 130, 15, TFT_WHITE);
  tft.setCursor(70, 70);
  tft.println("speed:");
  tft.setCursor(70, 120);
  tft.print(NEO6.speed.kmph(), 2); //  (millis() / (10 * 60 * 60)) % 24 NEO6.time.hour()
  tft.print("km/h");

  tft.drawRoundRect(50, 230, 220, 200, 15, TFT_WHITE);
  tft.setCursor(70, 270);
  tft.println("Sat inf:");
  tft.setCursor(70, 320);
  tft.print(NEO6.satellites.value()); //  (millis() / (10 * 60)) % 60 NEO6.time.minute()
}
void Screenthree()
{
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("fixe gauge later");
  /*
  currentMillis = millis();
  if(currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    speed.setZones(75, 100, 50, 75, 25, 50, 0, 25);
    speed.analogSpeedMeter(0,0, 3.0, "km/h", "0", "50", "100", "150", "200"); // Draw analogue meter at 0, 0
  }
  */
}
void ScreenFour()
{
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("fixe WS2812 later");
}
void ScreenFive()
{ //  rotation and Acceleration info
  currentMillis = millis();
  if (currentMillis - previousMillis >= 333)
  {
    previousMillis = currentMillis;

    tft.setTextSize(3);
    tft.drawRoundRect(20, 20, 280, 210, 15, TFT_WHITE);
    tft.setCursor(40, 50);
    tft.println("Roll: ");
    tft.setCursor(150, 50);
    tft.println(AnglePitch);
    tft.setCursor(40, 115);
    tft.println("Pitch: ");
    tft.setCursor(150, 115);
    tft.println(AngleRoll);
    tft.setCursor(40, 180);
    tft.println("Yaw: ");
    tft.setCursor(150, 180);
    tft.println("NaN"); //  no magnetometer so no yaw

    tft.drawRoundRect(20, 250, 280, 210, 15, TFT_WHITE);
    tft.setCursor(40, 280);
    tft.println("X[g]: ");
    tft.setCursor(150, 280);
    tft.println(AccX);
    tft.setCursor(40, 345);
    tft.println("Y[g]: ");
    tft.setCursor(150, 345);
    tft.println(AccY);
    tft.setCursor(40, 410);
    tft.println("Z[g]: ");
    tft.setCursor(150, 410);
    tft.println(AccZ);
  }
}
void ScreenSix()
{
  tft.setTextSize(3);
  currentMillis2 = millis();

  tft.setCursor(35, 50);
  tft.println("SD Card Error:");
  tft.setCursor(35, 100);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK, true);
  tft.println("Start Tracking:");
  tft.fillRoundRect(106, 180, 108, 150, 15, TFT_YELLOW);
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);

  if (SD.open("/"))
  {
    if (currentMillis2 - previousMillis2 >= 333)
    {
      previousMillis2 = currentMillis2;
      tft.drawRoundRect(20, 20, 280, 440, 15, TFT_WHITE);
      if (Tracking)
      {
        tft.setCursor(40, 50);
        tft.println("Stop Tracking: ");
        tft.fillRoundRect(106, 180, 108, 150, 15, TFT_RED);
      }
      else
      {
        tft.setCursor(40, 50);
        tft.println("Start Tracking: ");
        tft.fillRoundRect(106, 180, 108, 150, 15, TFT_GREEN);
      }
    }
  }
  tft.setTextSize(4);
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
    // Screenthree();
    ScreenFive();
    break;
  case 4:
    // ScreenFour();
    ScreenSix();
    break;
  case 5:
    // ScreenFive();
    break;
  case 6:
    // ScreenSix();
    break;
  case 7:
    // ScreenSeven();
    break;
  case 8:
    // Screeneight();
    break;
  default:
    break;
  }
}

//  Handle Buttons/Touchscreen inputs
void updateCounter(int increment)
{
  NavCounter = (NavCounter + increment + 5) % 5; // Ensure counter stays within 0-9 range
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

    updateCounter(-1); // Add 1
  }
  else if (digitalRead(INPUT1) == 0 and digitalRead(INPUT1) != PrevState[1])
  {
    Serial.println("I1 pressed");
    edgedetected[1] = true;
    gen_edge_det = 1;

    updateCounter(-3); // Subtract 3
  }
  else if (digitalRead(INPUT2) == 0 and digitalRead(INPUT2) != PrevState[2])
  {
    Serial.println("I2 pressed");
    edgedetected[2] = true;
    gen_edge_det = 1;
    Tracking = !Tracking; //  Toggle tracking
  }
  else if (digitalRead(INPUT3) == 0 and digitalRead(INPUT3) != PrevState[3])
  {
    Serial.println("I3 pressed");
    edgedetected[3] = true;
    gen_edge_det = 1;

    updateCounter(1); // Subtract 1
  }
  else if (digitalRead(INPUT4) == 0 and digitalRead(INPUT4) != PrevState[4])
  {
    Serial.println("I4 pressed");
    edgedetected[4] = true;
    gen_edge_det = 1;

    updateCounter(3); // Add 3
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
      Tracking = !Tracking; //  Toggle tracking
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

      updateCounter(-1); // Add 1
    }
    if (y < 180 and y > 30 and x < 214 and x > 106 and !edgedetected[3]) //  Right
    {
      Serial.println("I3 pressed");
      edgedetected[3] = true;
      gen_edge_det = 1;

      updateCounter(1); // Subtract 1
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

//  WS2812 Fastled_Breaks
void Fastled_Breaks()
{
  //Serial.println("Breaking detected"); //  set all LEDs to red
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Green; // Green is red and red is green for some reason
  }
  if (AccY > 0.01) // 100th of 1G about 10gramms of breaking "Force"
  {
    FastLED.setBrightness(255);
    cBreakMillis3 = millis();
  }
  else if (AccY < -0.1 and millis() - cBreakMillis3 > 500)
  { // keep breaks on for 1/2 sec set all LEDs to green
    FastLED.setBrightness(32);
  }
  FastLED.show(); // Update the LEDs
}

//  Get IMU data and filter it
void gyroSignals(void)
{
  //  Get the gyro data Filter
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  //  conf output
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  //  Pull data from accel
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  //  conf and pull gyro
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read();
  int16_t GyroZ = Wire.read() << 8 | Wire.read();
  //  conf and pull temp
  Wire.beginTransmission(0x68);
  Wire.write(0x41);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 2);
  int16_t Temp = Wire.read() << 8 | Wire.read();

  //  convert to physical values [g] + added correction factor for gyro
  AccX = (float)AccXLSB / 4096.0 - 0.07;
  AccY = (float)AccYLSB / 4096.0 + 0.01;
  AccZ = (float)AccZLSB / 4096.0;
  //  calc acctual degres
  AngleRoll = atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * (180 / M_PI);
  AnglePitch = -atan(AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * (180 / M_PI);
  //  Yaw not really possible due to drift and no magnetometer
  //  calc Temp in deg C
  tempC = (float)Temp / 340.0 + 36.53;
}

//  SDcard functions
//  SD card testing
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
void createDir(fs::FS &fs, const char *path)
{
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
  }
}
//  Start a GPX file if card is present and writable
void start_file()
{
  hour = NEO6.time.hour();
  minute = NEO6.time.minute();
  second = NEO6.time.second();
  fileID = hour + minute + second;

  // Write out GPX file header
  dataFile = SD.open("/" + fileID + ".gpx", FILE_WRITE);
  dataFile.println("<?xml version=\"1.0\"?>");
  dataFile.println("<gpx>");
  dataFile.println("<trk>");
  dataFile.println("<name>" + fileID + " GPS Route</name>");
  dataFile.println("<extensions>");
  dataFile.println("<gpxx:TrackExtension xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\">");
  dataFile.println("<gpxx:DisplayColor>Black</gpxx:DisplayColor>");
  dataFile.println("</gpxx:TrackExtension>");
  dataFile.println("</extensions>");
  dataFile.println("<trkseg>");
}
void writeFile()
{
  // GPX file format
  dataFile.print("<trkpt lat=\"");
  dataFile.print(NEO6.location.lat(), 6);
  dataFile.print("\" lon=\"");
  dataFile.print(NEO6.location.lng(), 6);
  dataFile.println("\">");
  dataFile.print("<ele>");
  dataFile.print(NEO6.altitude.meters());
  dataFile.println("</ele>");
  dataFile.print("<time>");
  dataFile.print(year + "-" + month + "-" + day + "T");
  dataFile.print(hour + ":" + minute + ":" + second + "Z");
  dataFile.println("</time>");
  dataFile.println("</trkpt>");
}
void endfile()
{ // Write file end data for GPX format
  dataFile.println("</trkseg>");
  dataFile.println("</trk>");
  dataFile.println("</gpx>");
  dataFile.close();
}

//  Mainly for debuging and sanity checks
//  Just a blinky thing
void StillAlive()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= 100)
  {
    previousMillis = currentMillis;
    digitalWrite(2, !digitalRead(2));
  }
}
void GPSdebug()
{
  while (gpsSerial.available() > 0)
  {
    // get the byte data from the GPS
    char gpsData = gpsSerial.read();
    Serial.print(gpsData);
  }
}
void Tempdebug()
{
  //  Print the temp to serial for debuging
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.println(" C");
}

void gen_IO_SU()
{
  //  conf pins for generalIO
  pinMode(2, OUTPUT);     //  Built in led
  pinMode(26, OUTPUT);    //  Backlight pin for TFT
  digitalWrite(26, HIGH); //  Backlight on
  //  Pins for buttons
  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  //  Pins for TFT and SD card SPI and turn all spi devices off

    /*
  pinMode(TFT_CS, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  //  Setup SD
  SD_Setup();   // SD fix later conflict with TFT
  */
}
void SD_Setup()
{
  // Initialize SD card
  Serial.println("Initializing SD card...");
  digitalWrite(SD_CS, LOW); // Activate SD card
  delay(10);                // Allow SPI bus to stabilize
  if (!SD.begin(SD_CS))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  digitalWrite(SD_CS, HIGH); // Deactivate SD card
  Serial.println("SD card initialized successfully.");

  /*
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card found");
    return;
  }
  Serial.printf("SD card type: %d\n", cardType);
  Serial.printf("SD size: %lluMB\n", SD.cardSize() / (1024 * 1024));
  */

  //  delete after testing
  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  fakenema();
  Serial.println("SD card written successfully.");
}
void TFT_Touch_SU()
{
  tft.init();
  tft.fillScreen(TFT_BLACK);

  tft.setRotation(3);
  tft.fillScreen((TFT_WHITE));
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

  //  finalize tft setup
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  tft.setRotation(2);
  tft.setTextFont(1); // font 2 kinda nice
}
void DisplayLogo()
{
  // Allocate a buffer to hold the pixel data
  File file = SPIFFS.open("/TBLOGO.c", "r");
  uint16_t buffer[320]; // Adjust size based on your display width
  int y = 80;
  while (file.available())
  {
    // Starting Y position

    if (!file)
    {
      Serial.println("Failed to open file for reading");
      return;
    }
    file.read((uint8_t *)buffer, sizeof(buffer));
    tft.pushImage(0, y, 320, 1, buffer);
    y++;
  }
  file.close();
}
void IMU_Setup()
{
  Wire.setClock(400000); // Set I2C frequency to 400kHz
  Wire.begin(21, 22);    // SDA, SCL
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}
void WS2812_Setup()
{
  FastLED.clear(); // Clear all LEDs
  Serial.printf("FastLED Setup \n");
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(64);
  FastLED.show(); // Update the LEDs
}

void setup()
{
  //  Start serial comunication for debuging
  Serial.begin(115200);

  // Pins used for Buttons, alive Led and backlight
  gen_IO_SU();

  //  Setup SPI
  SPI.begin();

  //  Setup TFT and Touchscreen
  Serial.printf("Initializing TFT... \n");
  TFT_Touch_SU();

  //  Setup IMU
  Serial.printf("IMU Setup \n");
  IMU_Setup();

  //  Setup ws2812 led
  WS2812_Setup();

  //  Setup GPS

  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  if (gpsSerial.available())
  {
    Serial.print("Serial 2 at ");
    Serial.print(GPS_BAUD);
    Serial.print(" baud"); // => Dev_Debug(ReportGPSSetup);
  }

  //  print the logo on startup (1 second)    idfk wyh this does not work anymore
  // DisplayLogo();
  // delay(1000);
}

void loop()
{
  // fakenema();
  realNMEA(); //  get real GPS data

  // checkButtons(); //  Buttons work but i use touch cuz more practical
  HandleTouchscreen();
  ScreenNav();
  gyroSignals(); //  get gyro data and filter it
  Fastled_Breaks();
  HandlePrinting2File();

  // testing
  GPSdebug();
  //  StillAlive(); // just blink build in led 1Hz
}

/*
  start_file();
  writeFile();
  endfile();
*/