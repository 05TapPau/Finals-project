#include <Arduino.h>
#include <TFT_eSPI.h>        

TFT_eSPI tft = TFT_eSPI();

void setup()
{
             
  tft.init();  

}

void loop()
{
  tft.init();
  tft.setRotation(3);
  tft.setCursor(49,30);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(5);
  tft.fillScreen(0xFC00); 
  delay(1000);     
  
  tft.print("Z");
  delay(300);
  tft.print("I");
  delay(300);
  tft.print("O");
  delay(300);
  tft.print(" ");
  delay(300);
  tft.print("T");
  delay(300);
  tft.print("E");
  delay(300);
  tft.print("S");
  delay(300);
  tft.print("T");
  delay(300);
  tft.print("E");
  delay(300);
  tft.print("R");
  delay(300);
  tft.print(".");
  delay(300);
  tft.print("I");
  delay(300);
  tft.println("T");
  delay(500);

  tft.drawRect(85,87,147,20,0xFFFF);tft.fillRect(85,87,147,20,0xFFFF);
  tft.drawChar(90,90,0x50,0xFC00,0xFFFF,2);delay(200);//P 
  tft.drawChar(100,90,0x4C,0xFC00,0xFFFF,2);delay(200);//L
  tft.drawChar(110,90,0x41,0xFC00,0xFFFF,2);delay(200);//A
  tft.drawChar(120,90,0x54,0xFC00,0xFFFF,2);delay(200);//T
  tft.drawChar(130,90,0x46,0xFC00,0xFFFF,2);delay(200);//F
  tft.drawChar(140,90,0x4F,0xFC00,0xFFFF,2);delay(200);//O
  tft.drawChar(150,90,0x52,0xFC00,0xFFFF,2);delay(200);//R
  tft.drawChar(160,90,0x4D,0xFC00,0xFFFF,2);delay(200);//M
  tft.drawChar(170,90,0x49,0xFC00,0xFFFF,2);delay(200);//I
  tft.drawChar(180,90,0x4F,0xFC00,0xFFFF,2);delay(200);//O
  tft.drawChar(190,90,0x2E,0xFC00,0xFFFF,2);delay(200);//.
  tft.drawChar(200,90,0x69,0xFC00,0xFFFF,2);delay(200);//i
  tft.drawChar(210,90,0x6E,0xFC00,0xFFFF,2);delay(200);//n
  tft.drawChar(220,90,0x69,0xFC00,0xFFFF,2);delay(200);//i
  delay(1000);

  tft.setCursor(25,150);
  tft.print("T");delay(250);tft.print("F");delay(250);tft.print("T");delay(250);tft.print(" ");tft.print("L");delay(250);tft.print("C");delay(250);tft.print("D");delay(250);tft.print(" ");tft.print("I");delay(250);tft.print("L");delay(250);tft.print("I");delay(250);tft.print("9");delay(250);tft.print("3");delay(250);tft.print("4");delay(250);tft.print("1");delay(250);
  tft.setCursor(200,230);tft.print("2");delay(250);tft.print(".");delay(250);tft.print("4");delay(250);tft.setTextSize(2.5);tft.print("''");
  delay (2000);
  tft.fillScreen(0xFFFF);
  delay(1000);
}