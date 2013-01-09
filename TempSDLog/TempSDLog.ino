/*
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 * RTC DS1307 attached to I2C bus as follows:
 ** SDA - pin 4
 ** SCL - pin 5
*/
#include <OneWire.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

int DS18S20_Pin = 2;
int SS_pin = 10;
int CS_pin = 10;
int read_delay = 60000;
char* filename = "temp.csv";
OneWire ds(DS18S20_Pin);
File myFile;
RTC_DS1307 RTC;

void setup(void) {
  Wire.begin();
  RTC.begin();
  Serial.begin(57600);
  pinMode(SS_pin, OUTPUT);
  if (!SD.begin(CS_pin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    Serial.println("Log file temp.txt found/created");
    myFile.close();
  } else {
    Serial.println("error opening temp.txt");
  }
}

void loop(void) {
  delay(read_delay);

  float temperature = getTemp();
  Serial.print("Read temperature: ");
  Serial.println(temperature);

  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    Serial.print("Writing temperature to log file...");
    myFile.print(getDateTime());
    myFile.print(",");
    myFile.println(temperature);
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening temp.txt");
  }
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,0); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
}

String getDateTime() {
  DateTime now = RTC.now();
  String str_datetime;
  int tmp;

  str_datetime = "";
  str_datetime += int(now.year());
  tmp = now.month();
  if (tmp < 10)
    str_datetime += "0";
  str_datetime += int(tmp);
  tmp = now.day();
  if (tmp < 10)
    str_datetime += "0";
  str_datetime += int(tmp);
  tmp = now.hour();
  if (tmp < 10)
    str_datetime += "0";
  str_datetime += int(tmp);
  tmp = now.minute();
  if (tmp < 10)
    str_datetime += "0";
  str_datetime += int(tmp);
  tmp = now.second();
  if (tmp < 10)
    str_datetime += "0";
  str_datetime += int(tmp);
  
  return str_datetime;
}

