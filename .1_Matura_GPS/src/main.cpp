#include <Arduino.h>
/*********
  Base code by Rui Santos & Sara Santos - Random Nerd Tutorials @ https://RandomNerdTutorials.com/esp32-neo-6m-gps-module-arduino/
  

  Modified and extended by Paul Tappeiner fitting for the finalsproject found @ https://github.com/05TapPau/Finals-project.git
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

 // Define the RX and TX pins for Serial 2
#define RXD2 17
#define TXD2 16

#define GPS_BAUD 9600

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

void setup(){
  // Serial Monitor
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  
  // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  if (gpsSerial.available())
  {
    Serial.print("Serial 2 at ");
    Serial.print(GPS_BAUD);
    Serial.print(" baud"); // => Dev_Debug(ReportGPSSetup);
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop(){
  while (gpsSerial.available() > 0)
  {
    // get the byte data from the GPS
    char gpsData = gpsSerial.read();
    Serial.print(gpsData);
  }
  delay(1000);
  digitalWrite(2, !digitalRead(2));
  Serial.println("-------------------------------");
}










