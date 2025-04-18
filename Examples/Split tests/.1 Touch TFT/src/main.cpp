/*
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

#include <Arduino.h>
#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

int ScreenNavCounter = 1;

#define CALIBRATION_FILE "/calibrationData"

bool edgedetected[5] = {0, 0, 0, 0, 0}; //  flags to recognize if there was a falling edge (only for 1 cycle)
bool PrevState[5] = {0, 0, 0, 0, 0};    // used to check for falling edges (Buttons pullups)
bool gen_edge_det = 0;                  // gets set if there was a edge detected no matter what pin

uint16_t x, y;
uint16_t calibrationData[5];
uint8_t calDataOK = 0;

/*
void setup()
{
  Serial.begin(115200);
  tft.init();
  tft.fillScreen(0xFC00);

  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
}
*/
void Homescreen()
{
  tft.setRotation(2);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(5);

  tft.setCursor(30, 30);
  tft.println("Lon: ");
  tft.setCursor(30, 90);
  tft.println("Lat: ");
  tft.setCursor(30, 150);
  tft.println("Alt: ");
  tft.setCursor(30, 210);
  tft.println("Speed: ");

  tft.setTextSize(5);
  tft.setCursor(60, 300);
  tft.print("10");
  tft.print(" km/h");
}
void leftscreen()
{
  tft.setRotation(3);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(5);

  tft.setCursor(30, 30);
  tft.println("this is a");
  tft.setCursor(30, 90);
  tft.println("second screen");
}
void rightscreen()
{
  tft.setRotation(3);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(5);

  tft.setCursor(30, 30);
  tft.println("u guessed it,");
  tft.setCursor(30, 90);
  tft.println("a third screen");
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("starting");

  tft.init();
  tft.setRotation(3);
  tft.fillScreen((0xFFFF));

  tft.setCursor(20, 0, 2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
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

  tft.fillScreen((0xFFFF));
  tft.setRotation(2);
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
}

void loop()
{
  // tft.fillTriangle(0,0,160,0,160,106,TFT_ORANGE);
  // tft.fillTriangle(320,0,320,106,480,0,TFT_ORANGE);
  // tft.fillTriangle(0,320,160,213,160,320,TFT_ORANGE);
  // tft.fillTriangle(320,213,320,320,480,320,TFT_ORANGE);
  // tft.fillTriangle(0,0,0,106,160,106,TFT_RED);
  // tft.fillTriangle(0,213,0,320,160,213,TFT_RED);
  // tft.fillTriangle(480,0,480,106,320,106,TFT_RED);
  // tft.fillTriangle(480,213,480,320,320,213,TFT_RED);

  // tft.fillRect(0, 0, 30, 320, TFT_BLACK);       // Infocenter?
  // tft.fillRect(180, 0, 150, 106, TFT_ORANGE);   // Right         -
  // tft.fillRect(180, 214, 150, 106, TFT_ORANGE); // Left          -
  // tft.fillRect(30, 106, 150, 108, TFT_RED);     // Up            -
  // tft.fillRect(330, 106, 150, 108, TFT_RED);    // Down          -
  // tft.fillRect(106, 180, 108, 150, TFT_BLACK);  // center        -
  // tft.fillRect(450, 214, 30, 108, TFT_BLACK);   // bottomleft

  //tft.fillRect(106, 30, 108, 150, TFT_YELLOW);  // Up
  //tft.fillRect(106, 180, 108, 150, TFT_CYAN);   // center
  //tft.fillRect(106, 330, 108, 150, TFT_RED);    // Down
  //tft.fillRect(0, 180, 106, 150, TFT_PURPLE);   // Left
  //tft.fillRect(214, 180, 106, 150, TFT_ORANGE); // Right

  if (tft.getTouch(&x, &y))
  {

    // if (y < 30 and y > 0)
    //{
    //   tft.fillRect(0, 0, 320, 30, TFT_GREEN); // Infocenter?
    // }
    // if (x < 106 and x > 0 and y < 480 and y > 390)
    //{
    //   tft.fillRect(0, 390, 108, 30, TFT_GREEN);   // bottomleft return whatever
    // }

    if (x > 106 and x < 214 and y < 330 and y > 180 and !edgedetected[2])
    {
      Serial.println("I2 pressed");
      edgedetected[2] = true;
      gen_edge_det = 1;

      tft.fillRect(106, 180, 108, 150, TFT_GREEN); // center
    }
    if (x < 106 and x > 0 and y < 330 and y > 180 and !edgedetected[4])
    {
      Serial.println("I4 pressed");
      edgedetected[4] = true;
      gen_edge_det = 1;

      tft.fillRect(106, 30, 108, 150, TFT_GREEN); // Up
    }
    if (x < 320 and x > 214 and y < 330 and y > 180 and !edgedetected[1])
    {
      Serial.println("I1 pressed");
      edgedetected[1] = true;
      gen_edge_det = 1;

      tft.fillRect(106, 330, 108, 150, TFT_GREEN); // Down
    }
    if (y < 480 and y > 330 and x < 214 and x > 106 and !edgedetected[0])
    {
      Serial.println("I0 pressed");
      edgedetected[0] = true;
      gen_edge_det = 1;

      if (ScreenNavCounter < 2)
      {
        ScreenNavCounter++;
      }
      else
        ScreenNavCounter = 0;

      tft.fillRect(0, 180, 106, 150, TFT_GREEN); // Left
    }
    if (y < 180 and y > 30 and x < 214 and x > 106 and !edgedetected[3])
    {
      Serial.println("I3 pressed");
      edgedetected[3] = true;
      gen_edge_det = 1;

      tft.fillRect(214, 180, 106, 150, TFT_GREEN); // Right

      if (ScreenNavCounter > 0)
      {
        ScreenNavCounter--;
      }
      else
        ScreenNavCounter = 2;
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

    // tft.fillRect(0,   0,    30,   320, TFT_BLACK);      // Infocenter? not pushed
    //   tft.fillRect(450, 214,  30,   108, TFT_BLACK);      // bottomleft
    tft.fillRect(106, 30, 108, 150, TFT_BLACK);  // Up
    tft.fillRect(106, 180, 108, 150, TFT_BLACK); // center
    tft.fillRect(106, 330, 108, 150, TFT_BLACK); // Down
    tft.fillRect(0  , 180, 106, 150, TFT_BLACK); // Left
    tft.fillRect(214, 180, 106, 150, TFT_BLACK); // Right
  }

  if (tft.getTouch(&x, &y))
  {
    tft.setCursor(40, 5, 2);
    tft.printf("x: %i     ", x);
    tft.setCursor(40, 40, 2);
    tft.printf("y: %i    ", y);
  }
}