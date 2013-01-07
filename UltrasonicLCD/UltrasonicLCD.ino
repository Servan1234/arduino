#include <LiquidCrystal.h>

#define trigPin 8
#define echoPin 9

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin (57000);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.begin(16, 2);
  lcd.print("Ultrasonic sensor");
  Serial.println("Ultrasonic sensor");
}

void loop() {
  int duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  lcd.clear();
  lcd.print("Distance:");
  lcd.setCursor(0, 1);

  if (distance >= 200 || distance <= 0){
    lcd.print("Out of range");
    Serial.println("Out of range");
  }
  else {
    lcd.print(distance);
    lcd.print(" cm");
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}



