#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

File root;
File dataFile;
String fileID;

#include "TinyGPS++.h"

HardwareSerial gpsSerial(2);
TinyGPSPlus NEO6;

#define RXD2 17
#define TXD2 16
#define GPS_BAUD 9600
String year, month, day, hour, minute, second;

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

const char *gpsStream =
    "$GPGGA,135135.130,4640.778,N,01109.212,E,1,12,1.0,0.0,M,0.0,M,,*6A\r\n"
    "$GPRMC,135135.130,A,4640.778,N,01109.212,E,,,030325,000.0,W*79\r\n";

void fakenema()
{
  // get fake nema info for gps cuz gps wont get a fix
  while (*gpsStream)
    NEO6.encode(*gpsStream++);
  delay(100);
}

void setup()
{
  Serial.begin(115200);
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

  listDir(SD, "/", 0);
  createDir(SD, "/mydir");

  fakenema();
  
    /*this creats a file, names it writes data and closes it
      start_file();
      writeFile();
      endfile();
    */
}

void loop()
{
}