/*********
  Code by Paul Tappeiner fitting for the finalsproject found @ https://github.com/05TapPau/Finals-project.git
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

// only used for working with VScode/PlatformIO withint the Arduino framework
#include <Arduino.h>

// Library for comunicating via I2C used for IMU
#include <Wire.h>

// Library for comunicating via SPI used for SD-Card, TFT-LCD (&Touch)
#include "SPI.h"

// Library for navigating Memory (SD-Card)
#include "FS.h"

// Library for navigating SD-Cards and getting information from it(Data & general info) (via SPI)
#include "SD.h"

// Library for MPU6050
#include <Adafruit_MPU6050.h>

// RX and TX pins and Speed of Serial 2
#define RXD2 17
#define TXD2 16
#define GPS_BAUD 9600

// Create 2nd instance of HardwareSerial class for Serial 2 and call it gps cuz its for gps
HardwareSerial gpsSerial(2);

// Create instance of MPU
Adafruit_MPU6050 mpu;

//


/*Futureprooving stuff
enum DevReports
{
  StartDebugSerial,
  ReportGPSSetup,
  ReportGPS,
  ReportIMUSetup,
  ReportIMU,
  ReportSD,
  ReportTFT,
  ReportDivision
};
void Dev_Debug(int reportrequest){
  switch (reportrequest)
  {
  case StartDebugSerial:
    Serial.begin(115200);
    break;
  case ReportGPSSetup:
    Serial.print("Serial 2 started at ");
    Serial.print(GPS_BAUD);
    Serial.print(" baud rate");
    break;
  case ReportGPS:
    break;
  case ReportIMUSetup:
    while (!Serial)
    {
      delay(10);
    }
    Serial.println("MPU6050 test");
    break;
  case ReportIMU:
    break;
  case ReportSD:
    break;
  case ReportTFT:
    break;
  case ReportDivision:
    Serial.println("--------------------------------");
    break;
  default:
    break;
  }
}
*/

void setup()
{
  Serial.begin(115200); // => Dev_Debug(StartDebugSerial);
  while (!Serial)
  delay(10); // will pause until serial console opens

  // Setup GPS
  // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  if (gpsSerial.available())
  {
    Serial.print("Serial 2 at ");
    Serial.print(GPS_BAUD);
    Serial.print(" baud"); // => Dev_Debug(ReportGPSSetup);
  }


  // Setup IMU
  Serial.println("Adafruit MPU6050 test!");
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");


  //  Setup SD card
  if (!SD.begin(5))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }
  delay(100);
}


void GetGPS_Data()
{
  while (gpsSerial.available() > 0)
  {
    // get the byte data from the GPS
    char gpsData = gpsSerial.read();
    Serial.print(gpsData);
  }
}
void GetIMU_Data()
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print(g.gyro.x);
  Serial.print("\t");
  Serial.print(g.gyro.y);
  Serial.print("\t");
  Serial.print(g.gyro.z);
  Serial.println("\t");
}
void Hanlde_SD_Card()
{
}
void Handle_TFT(){}

void loop()
{
  GetGPS_Data();
}
