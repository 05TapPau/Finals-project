#include <Arduino.h>
#include <TFT_eSPI.h>

#include <TinyGPSPlus.h>

//  IO Buttons (5Way)
#define INPUT0 34
#define INPUT1 35
#define INPUT2 32
#define INPUT3 36
#define INPUT4 39

//  Serial coms GPS
#define RXD2 17
#define TXD2 16
#define GPS_BAUD 9600

HardwareSerial gpsSerial(2);
TinyGPSPlus NEO6;
TFT_eSPI tft = TFT_eSPI();

// A sample NMEA stream cuz GPS wont get a fix ffs
int simSeconds = 0;
const char *gpsStream =
    "$GPGGA,135135.130,4640.778,N,01109.212,E,1,12,1.0,0.0,M,0.0,M,,*6A\r\n"
    "$GPRMC,135135.130,A,4640.778,N,01109.212,E,,,030325,000.0,W*79\r\n";

int ScreenNavCounter = 1;
bool prevIn0 = 1, prevIn1 = 1, prevIn2 = 1, prevIn3 = 1, prevIn4 = 1;


void Homescreen()
{
  tft.setRotation(2);
  tft.fillScreen(0xFC00);
  tft.setTextColor(0xFFFF,0xFC00,true);
  tft.setTextSize(7);
  //tft.setTextFont(1); // font 2 kinda nice

  tft.setCursor(120, 10);
  tft.println(NEO6.time.hour());
  tft.print("  ");
  tft.setCursor(120, 70);
  tft.println(NEO6.time.minute());
  tft.setCursor(120, 130);
  tft.println(simSeconds/*NEO6.time.second()*/);

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
  tft.fillScreen(0xFC00);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(5);

  tft.setCursor(10, 10);
  tft.println("Lon: ");
  tft.setCursor(10, 60);
  tft.print(NEO6.location.lng(),3);
  tft.setCursor(10, 110);
  tft.println("Lat: ");
  tft.setCursor(10, 160);
  tft.print(NEO6.location.lat(),3);
  tft.setCursor(10, 210);
  tft.println("Alt: ");
  tft.setCursor(10, 260);
  tft.print(NEO6.altitude.meters(),3);
  tft.setCursor(10, 310);
  tft.println("Speed: ");
  tft.setCursor(40, 360);
  tft.print(NEO6.speed.kmph());
  tft.print(" km/h");
}

void rightscreen()
{
  tft.setRotation(3);
  tft.fillScreen(0xFC00);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(5);

  tft.setCursor(30, 30);
  tft.println("u guessed it,");
  tft.setCursor(30, 90);
  tft.println("a third screen");
}

void checkbuttons()
{
  if (digitalRead(INPUT0) == 0 and digitalRead(INPUT0) != prevIn0)
  {
    Serial.println("I0 pressed");

    if (ScreenNavCounter > 0)
    {
      ScreenNavCounter--;
    }
    else
      ScreenNavCounter = 2;
  }
  prevIn0 = digitalRead(INPUT0);
  if (digitalRead(INPUT1) == 0 and digitalRead(INPUT1) != prevIn1)
  {
    Serial.println("I1 pressed");
    if (ScreenNavCounter < 2)
    {
      ScreenNavCounter++;
    }
    else
      ScreenNavCounter = 0;
  }
  prevIn1 = digitalRead(INPUT1);
  if (digitalRead(INPUT2) == 0 and digitalRead(INPUT2) != prevIn2)
  {
    Serial.println("I2 pressed");
  }
  prevIn2 = digitalRead(INPUT2);
  if (digitalRead(INPUT3) == 0 and digitalRead(INPUT3) != prevIn3)
  {
    Serial.println("I3 pressed");
  }
  prevIn3 = digitalRead(INPUT3);
  if (digitalRead(INPUT4) == 0 and digitalRead(INPUT4) != prevIn4)
  {
    Serial.println("I4 pressed");
  }
  prevIn4 = digitalRead(INPUT4);
}

void setup()
{
  tft.init();
  tft.fillScreen(0x0000);

  Serial.begin(115200);
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");

  pinMode(INPUT0, INPUT);
  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
}

void fakenema (){
  // get fake nema info for gps cuz gps wont get a fix
  while (*gpsStream)
    NEO6.encode(*gpsStream++);
  
  if (simSeconds==60)
    simSeconds=0;
  else
  simSeconds++;

  Serial.println(simSeconds);
  delay(100);
}

void NavFrames(){
  checkbuttons();

  // screennavigation
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

void loop()
{
  Homescreen();
  /*
  while (gpsSerial.available() > 0)
  {
    // get the byte data from the GPS
    char gpsData = gpsSerial.read();
    Serial.print(gpsData);
  }

  Serial.println("-------------------------------");
  */
 delay(1000);
}
