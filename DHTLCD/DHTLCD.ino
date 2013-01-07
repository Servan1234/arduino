#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 10
#define DHTTYPE DHT11

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.begin(16, 2);
  lcd.print("DHT11 test");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear();

  if (isnan(t) || isnan(h)) {
    lcd.print("Failed to read");
    lcd.setCursor(0, 1);
    lcd.print("from DHT");
  } else {
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print(" ");
    lcd.print((char)223);
    lcd.print("C");
  }

  delay(2000);
}
