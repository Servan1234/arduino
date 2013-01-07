#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int mnt,sec;

void setup() {
  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  lcd.print("RTC LCD Clock");
  lcd.setCursor(0, 1);
  lcd.print("version 1.01");
  delay(1500);
}

void loop() {
  DateTime now = RTC.now();

  lcd.clear();
  lcd.print(now.day(), DEC);
  lcd.print('-');
  lcd.print(now.month(), DEC);
  lcd.print('-');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  mnt = now.minute();
  if (mnt < 10)
    lcd.print("0");
  lcd.print(mnt, DEC);
  lcd.print(':');
  sec = now.second();
  if (sec < 10)
    lcd.print("0");
  lcd.print(sec, DEC);

  delay(1000);
}
