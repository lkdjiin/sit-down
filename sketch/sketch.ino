// Sit down! version 1.0.1
//
// SD card module
// ==============
// CS   - 10
// MOSI - 11
// MISO - 12
// CLK  - 13
//
// Real time clock module
// ======================
// SDA - A4
// SCL - A5

#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>

const int chipSelect = 10;
const int PRESSION = A0;
const int ERROR_LED = 8;
const int WRITE_LED = 7;

RTC_DS1307 RTC;

void setup() {
  pinMode(PRESSION, INPUT);
  pinMode(ERROR_LED, OUTPUT);
  pinMode(WRITE_LED, OUTPUT);

  Wire.begin();

  if(!RTC.isrunning() || !SD.begin(chipSelect)) {
    digitalWrite(ERROR_LED, HIGH);
    return;
  }
}

void loop() {
  digitalWrite(ERROR_LED, LOW);
  digitalWrite(WRITE_LED, LOW);
  int sensor = analogRead(PRESSION);
  String dataString = String(sensor);
  DateTime now = RTC.now();

  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  if(dataFile) {
    printDatePart(dataFile, now.year(), "-");
    printDatePart(dataFile, now.month(), "-");
    printDatePart(dataFile, now.day(), " ");
    printDatePart(dataFile, now.hour(), ":");
    printDatePart(dataFile, now.minute(), ":");
    printDatePart(dataFile, now.second(), ",");
    dataFile.println(dataString);

    dataFile.close();
    digitalWrite(WRITE_LED, HIGH);
    delay(1000);
    digitalWrite(WRITE_LED, LOW);
    delay(29000);
  } else {
    digitalWrite(ERROR_LED, HIGH);
    delay(30000);
  }

}

void printDatePart(File file, int value, String separator) {
  maybePrintZero(file, value);
  file.print(value, DEC);
  file.print(separator);
}

void maybePrintZero(File file, int testValue) {
  if(testValue < 10) {
    file.print("0");
  }
}
