#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int ScreenNavCounter = 1;

bool prevIn0=0, prevIn1=0, prevIn2=0, prevIn3=0, prevIn4=0;

#define INPUT0 34 //  add direction
#define INPUT1 35 //  add direction
#define INPUT2 32 //  add direction
#define INPUT3 36 //  add direction  
#define INPUT4 39 //  add direction

void setup()
{
  Serial.begin(115200);
  tft.init();
  tft.fillScreen(0xFC00);

  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
}

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

void checkbuttons(){
  if (digitalRead(INPUT0) != prevIn0)
  {
    tft.fillScreen(0xFC00);
    prevIn0 = digitalRead(INPUT0);
    Serial.println("I0 pressed");

    if (ScreenNavCounter > 0)
    {
      ScreenNavCounter--;
    }
    else
      ScreenNavCounter = 2;
  }
  if (digitalRead(INPUT1) != prevIn1)
  {
    tft.fillScreen(0xFC00);
    prevIn1 = digitalRead(INPUT1);
    Serial.println("I1 pressed");
    if (ScreenNavCounter < 2)
    {
      ScreenNavCounter++;
    }
    else
      ScreenNavCounter = 0;
  }
  if (digitalRead(INPUT2) != prevIn2)
  {
    tft.fillScreen(0xFC00);
    prevIn1 = digitalRead(INPUT2);
    Serial.println("I2 pressed");
    if (){}
    else
  }
  if (digitalRead(INPUT3) != prevIn3)
  {
    tft.fillScreen(0xFC00);
    prevIn1 = digitalRead(INPUT3);
    Serial.println("I3 pressed");
    if (){}
    else
  }
  if (digitalRead(INPUT4) != prevIn4)
  {
    tft.fillScreen(0xFC00);
    prevIn1 = digitalRead(INPUT4);
    Serial.println("I4 pressed");
    if (){}
    else
  }
}

void loop()
{
  void checkbuttons();
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


