//========== ex9-2-1.ino ============
// Arduino connections:
// Pin 7 - SERIAL RGB LED
// Pins 10 (RX) and 11 (TX) - Bluetooth
// Pin 12 - HUMIDITY/TEMPERATURE SENSOR

#include "DHT.h"                    // DHT sensor library
#include <SoftwareSerial.h>         // SoftwareSerial library for Bluetooth
#include <Adafruit_NeoPixel.h>      // Adafruit NeoPixel library for RGB LED

//========== Global Declarations ============
const int RGB_PIN = 7;              // SERIAL RGB LED pin
const int RX_PIN = 10;              // Bluetooth RxD
const int TX_PIN = 11;              // Bluetooth TxD
const int DHT_PIN = 12;             // DHT11 sensor pin

DHT myDHT(DHT_PIN, DHT11);          // Initialize DHT sensor
SoftwareSerial myBT(RX_PIN, TX_PIN); // Initialize Bluetooth
Adafruit_NeoPixel LED(20, RGB_PIN, NEO_GRB + NEO_KHZ800);

float temp, temp0 = 0;              // Variables for temperature tracking

//========== Initialization ============
void setup() {
    myBT.begin(9600);               // Start Bluetooth communication at 9600 bps
    pinMode(RGB_PIN, OUTPUT);       // Set RGB_PIN as output for RGB LED
    LED.begin();                    // Initialize NeoPixel library
    LED.show();                     // Reset all LEDs to off
}

//========== Main Loop ============
void loop() {
    readAndSendTemperature();
    handleBluetoothInstructions();
}

// Function to read temperature and send it if changed
void readAndSendTemperature() {
    temp = myDHT.readTemperature(); // Read temperature in Celsius
    if (temp != temp0) {            // Check for change in temperature
        myBT.write(temp);           // Send new temperature over Bluetooth
        temp0 = temp;               // Update previous temperature
    }
    delay(100);                     // Short delay for stability
}

// Function to handle Bluetooth instructions
void handleBluetoothInstructions() {
    if (myBT.available()) {                // Check if instruction received
        char instruction = myBT.read();    // Read instruction
        executeInstruction(instruction);   // Execute based on instruction
    }
}

// Execute RGB animations based on Bluetooth instruction
void executeInstruction(char instruction) {
    switch (instruction) {
        case 'A': knight(); break;
        case 'B': wiper(); break;
        case 'C': eyebrows(); break;
        case 'D': frog(); break;
        case 'E': rainbow1(50, 8); break;   // 8 cycles of rainbow1
        case 'F': rainbow2(3, 50); break;   // 3 cycles of rainbow2
    }
}

//========== Animation Functions ================

// Knight Rider animation
void knight() {
    pili1(LED.Color(50, 0, 0), 50); // red
    pili1(LED.Color(0, 50, 0), 50); // green
    pili1(LED.Color(0, 0, 50), 50); // blue
}

// Wiper effect
void wiper() {
    for (int i = 0; i < 3; i++) {
        pili2(LED.Color(30, 0, 0), 100);
        pili2(LED.Color(0, 30, 0), 100);
        pili2(LED.Color(0, 0, 30), 100);
    }
}

// Eyebrows effect
void eyebrows() {
    for (int i = 0; i < 3; i++) {
        pili3(LED.Color(30, 0, 0), 50);
        pili3(LED.Color(0, 30, 0), 50);
        pili3(LED.Color(0, 0, 30), 50);
    }
}

// Frog effect
void frog() {
    for (int i = 0; i < 3; i++) {
        jumping(LED.Color(30, 0, 0), 65);
        jumping(LED.Color(0, 30, 0), 65);
        jumping(LED.Color(0, 0, 30), 65);
    }
}

//========== LED Effect Helper Functions ================

// Single LED movement effect
void pili1(uint32_t color, uint8_t delayTime) {
    // Move LED from left to right
    for (int i = 0; i < LED.numPixels(); i++) {
        LED.setPixelColor(i, color);
        LED.show();
        delay(delayTime);
        LED.setPixelColor(i, 0);
    }
    // Move LED from right to left
    for (int i = 0; i < LED.numPixels(); i++) {
        LED.setPixelColor(LED.numPixels() - 1 - i, color);
        LED.show();
        delay(delayTime);
        LED.setPixelColor(LED.numPixels() - 1 - i, 0);
    }
    clearLED();
}

// Center-out wiper effect
void pili2(uint32_t color, uint8_t delayTime) {
    for (int i = 0; i < LED.numPixels() / 2; i++) {
        LED.setPixelColor(i, color);
        LED.setPixelColor(LED.numPixels() - 1 - i, color);
        LED.show();
        delay(delayTime);
    }
    for (int i = 0; i < LED.numPixels() / 2; i++) {
        LED.setPixelColor(LED.numPixels() / 2 - 1 - i, 0);
        LED.setPixelColor(LED.numPixels() - 1 - i, 0);
        LED.show();
        delay(delayTime);
    }
    clearLED();
}

// Eyebrows effect
void pili3(uint32_t color, uint8_t delayTime) {
    for (int i = 0; i < LED.numPixels() / 2; i++) {
        LED.setPixelColor(LED.numPixels() / 2 - 1 - i, color);
        LED.setPixelColor(LED.numPixels() / 2 + i, color);
        LED.show();
        delay(delayTime);
        LED.setPixelColor(LED.numPixels() / 2 - 1 - i, 0);
        LED.setPixelColor(LED.numPixels() / 2 + i, 0);
    }
    clearLED();
}

// Jumping LED effect
void jumping(uint32_t color, uint8_t delayTime) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < LED.numPixels() / 2; j += 3) {
            LED.setPixelColor(LED.numPixels() / 2 - 1 - j, color);
            LED.setPixelColor(LED.numPixels() / 2 + j, color);
        }
        LED.show();
        delay(delayTime);
        for (int j = 0; j < LED.numPixels() / 2; j += 3) {
            LED.setPixelColor(LED.numPixels() / 2 - 1 - j, 0);
            LED.setPixelColor(LED.numPixels() / 2 + j, 0);
        }
    }
    clearLED();
}

// Rainbow effect with static colors
void rainbow1(uint8_t delayTime, uint8_t cycles) {
    for (int j = 0; j < 256; j += 8) {
        LED.setBrightness(j / 2);
        for (int k = 0; k < LED.numPixels(); k++) {
            LED.setPixelColor(k, Wheel((k * 256 / LED.numPixels()) & 255));
        }
        LED.show();
        delay((255 - j) / 3);
    }
    clearLED();
}

// Rainbow effect with dynamic colors
void rainbow2(uint8_t counts, uint8_t delayTime) {
    for (int i = 0; i < counts; i++) {
        LED.setBrightness((i + 1) * 25);
        for (int j = 0; j < 256; j++) {
            for (int k = 0; k < LED.numPixels(); k++) {
                LED.setPixelColor(k, Wheel(((k * 256 / LED.numPixels()) + j) & 255));
            }
            LED.show();
            delay(delayTime);
        }
    }
    clearLED();
}

//========== Utility Functions ================

// Generate color based on wheel position
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) return LED.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    else if (WheelPos < 170) return LED.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    else return LED.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Clear all LEDs
void clearLED() {
    for (int i = 0; i < LED.numPixels(); i++) LED.setPixelColor(i, 0);
    LED.show();
}

