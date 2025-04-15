
#include <SPI.h>
#include <SD.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>


#define chipSelect 4
SoftwareSerial shield(8, 9); // configure software serial port 
static void smartdelay(unsigned long ms);
TinyGPS gps;

//String sfilename, filename;
char filename1[20];
char date1[22];
char filepath[20];
unsigned long start;
 float latitude, longitude;
 int year, h_dop;
 byte month, day, hour, minute, second;
 byte loopA;
File dataFile;
//boolean pereriv=1;

void setup()
{
  pinMode(10, OUTPUT);
  shield.begin(9600);
  Serial.begin(9600);
   if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
   }
   else {Serial.println("SD card is ready");
   }
   if (shield.available()){
   gps.encode(shield.read());
  unsigned long h_dop=gps.hdop();
   }
   
   do { Serial.println("wait");
        smartdelay(1000);
        h_dop=gps.hdop();
        Serial.println(h_dop);
   } while (h_dop ==  TinyGPS::GPS_INVALID_HDOP); //the track record does not start until gps fix
  
   gps.crack_datetime(&year,&month,&day,&hour,&minute,&second);
    hour = hour + 3;
    if (hour > 23) {hour = hour - 24; day +=1;}
    sprintf(filename1, "/%02d-%02d-%02d", day, month, year-2000);//название папки
    sprintf(filepath, "/%02d-%02d-%02d/%02d-%02d%s",day, month, year-2000,  hour, minute, ".GPX");//путь к файлу в папке
    Serial.println(filepath);
    SD.mkdir(filename1);
    
    if (!SD.exists(filepath)){
   dataFile = SD.open(filepath, FILE_WRITE);//создание файла 
   dataFile.print(F(
     "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
    "<gpx version=\"1.1\" creator=\"Batuev\" xmlns=\"http://www.topografix.com/GPX/1/1\" \r\n"
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\r\n"
    "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">\r\n"
    "\t<trk>\r\n<trkseg>\r\n"));  //heading of gpx file 
    dataFile.print(F("</trkseg>\r\n</trk>\r\n</gpx>\r\n"));
    dataFile.close();
 }}
 
 void loop() {
    float falt, h_dop1 ;
    gps.crack_datetime(&year,&month,&day,&hour,&minute,&second);
    gps.f_get_position(&latitude, &longitude); 
 falt = gps.altitude()/100;
 h_dop1 = gps.hdop();
 h_dop1 = h_dop1 /100;
 int spd = gps.f_speed_kmph();
 hour = hour + 3; //set your time zone, my zone is +3
       if (hour > 23) {hour = hour - 24; day +=1;}
       year -= 2000;
    sprintf(date1, "%4d-%02d-%02dT%02d:%02d:%02dZ",year, month, day, hour,minute,second);
     Serial.println(date1);
  if ( start > 2000) //delay for start
  {
    dataFile = SD.open(filepath, FILE_WRITE);
    unsigned long filesize = dataFile.size();
     // back up the file pointer to just before the closing tags
    filesize -= 27;
    dataFile.seek(filesize);
    dataFile.print(F("<trkpt lat=\"")); 
    dataFile.print(latitude,7);
    dataFile.print(F("\" lon=\""));
    dataFile.print(longitude,7);
    dataFile.println(F("\">"));
    dataFile.print(F("<time>"));
    dataFile.print(date1);
    dataFile.println(F("</time>"));      
    dataFile.print(F("<ele>")); 
    dataFile.print(falt,1);
    dataFile.print(F("</ele>\r\n<hdop>")); 
    //dataFile.print(F("<hdop>")); 
    dataFile.print(h_dop1,3);
    dataFile.println(F("</hdop>\r\n</trkpt>"));
    dataFile.print(F("</trkseg>\r\n</trk>\r\n</gpx>\r\n"));
    dataFile.close();
}
smartdelay(1000); //writing gps point every 1 sec
}

static void smartdelay(unsigned long ms) {
  start = millis();
  do 
  {
    while (shield.available())
      gps.encode(shield.read());
  } while (millis() - start < ms);
}


