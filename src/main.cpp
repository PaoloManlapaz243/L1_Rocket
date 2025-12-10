#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <SD.h>

#define SPISCK 18 //IO18
#define SPIMOSI 23 //IO23
#define SPIMISO 19 //IO19

#define CSBMP280 33 //IO33
#define CSSD 5 //IO5

#define ONBOARDLED 2

const int BMP280SPIHz = 10000000;
const int SDSPIHz = 4000000;
const int BMP280SampleHz = 1;

String fileName;
const char CONNECTEDCHECK[] = "/connected.txt";
const char FILEPREF[] = "/data";
const char FILESUF[] = ".csv";

File dataFile;

Adafruit_BME280 bmp(CSBMP280, &SPI);

void setup() 
{

  pinMode(CSBMP280, OUTPUT);
  pinMode(CSSD, OUTPUT);
  pinMode(ONBOARDLED, OUTPUT);
  
  digitalWrite(CSBMP280, HIGH);
  digitalWrite(CSSD, HIGH);
  digitalWrite(ONBOARDLED, HIGH);

  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_NONE,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_NONE,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */
                  
  bmp.begin();
  SD.begin(CSSD);

  Serial.begin(9600);


  digitalWrite(CSSD, LOW);
  File connectedFlag = SD.open(CONNECTEDCHECK, FILE_WRITE);

  //Keep trying to open the file
  while(!connectedFlag)
  {
    Serial.println("Cannot Access Drive");
    delay(100);
    connectedFlag = SD.open(CONNECTEDCHECK, FILE_WRITE);
  }
  connectedFlag.close();

  int i = 1;
  do
  {
    fileName = FILEPREF + String(i) + FILESUF;

    i++;
  } while(SD.exists(fileName));

  File dataFile = SD.open(fileName, FILE_WRITE);
  while(!dataFile)
  {
    Serial.println("Cannot Create Trial File");
    delay(100);
    dataFile = SD.open(fileName, FILE_WRITE);
  }
  dataFile.close();

  Serial.println("Writing to " + fileName);

  digitalWrite(CSSD, HIGH);

  digitalWrite(CSBMP280, LOW);

  while (!bmp.begin())
  {
    Serial.println("Can't talk to bmp");
    delay(100);
  }

  digitalWrite(CSBMP280, HIGH);

  digitalWrite(ONBOARDLED, LOW);
}

void loop() 
{
    digitalWrite(CSBMP280, LOW);
    float t = bmp.readTemperature();
    float p = bmp.readPressure();
    float h = bmp.readAltitude(1013.25);
    digitalWrite(CSBMP280, HIGH);


    t = t * 1.8 + 32;

    //SPI.beginTransaction(SPISettings(SDSPIHz, MSBFIRST, SPI_MODE0));
    digitalWrite(CSSD, LOW);

    File dataFile = SD.open(fileName, FILE_APPEND);

    // if the file is available, write to it:
    if (dataFile)
    {
      digitalWrite(ONBOARDLED, LOW);
      dataFile.print(millis());
      dataFile.print(",");
      dataFile.print(t);
      dataFile.print(",");
      dataFile.print(p);
      dataFile.print(",");
      dataFile.println(h);
      Serial.println(t);
    }
    // if the file isn't open, pop up an error:
    else 
    {
      digitalWrite(ONBOARDLED, HIGH);
      Serial.println("error opening the thing");
    }

    digitalWrite(CSSD, HIGH);
    //SPI.endTransaction();

    delay(250);

}
