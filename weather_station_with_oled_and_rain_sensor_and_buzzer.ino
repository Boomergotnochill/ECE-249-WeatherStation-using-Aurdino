#include "DHT.h"
#define DHT11Pin 2                // DHT pin connected to digital pin 2
#define DHTType DHT11
#define buzzerPin 13

// Rain Sensor
const int rainSensorPin = 3;  // Assuming digital input from the rain sensor connected to digital pin 3
bool isRaining = false;

// OLED
#include <Wire.h>         // for i2c communication
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

DHT HT(DHT11Pin, DHTType);
float humi;
float tempC;
float tempF;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT  64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  HT.begin();

  pinMode(rainSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(400);
  display.clearDisplay();
}

void loop() {
  delay(400);

  humi = HT.readHumidity();
  tempC = HT.readTemperature();
  tempF = HT.readTemperature(true);

  // Read the rain sensor
  isRaining = digitalRead(rainSensorPin);

  Serial.print("Humidity:");
  Serial.print(humi, 0);
  Serial.print("%");
  Serial.print(" Temperature:");
  Serial.print(tempC, 1);
  Serial.print("C  ~ ");
  Serial.print(tempF, 1);
  Serial.println("F");

  Serial.print("Rain Sensor: ");
  Serial.println(isRaining ? "Raining" : "Not Raining");

  // Buzzer control
  if (isRaining) {
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  } else {
    digitalWrite(buzzerPin, HIGH);   // Turn on the buzzer
  }

  display.clearDisplay();
  oledDisplayHeader();

  oledDisplay(3, 5, 15, humi, "%");
  oledDisplay(2, 70, 16, tempC, "C");
  oledDisplay(2, 70, 44, tempF, "F");

  // Display Rain Sensor Value
  display.setTextSize(1.4);
  display.setTextColor(WHITE);
  display.setCursor(2, 49);
  display.print(isRaining ? "Not Raining" : "Raining");

  display.display();
}

void oledDisplayHeader() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Humidity");
  display.setCursor(60, 0);
  display.print("Temperature");
}
void oledDisplay(int size, int x, int y, float value, String unit) {
  int charLen = 12;
  int xo = x + charLen * 3.2;
  int xunit = x + charLen * 3.6;
  int xval = x;
  display.setTextSize(size);
  display.setTextColor(WHITE);

  if (unit == "%") {
    display.setCursor(x, y);
    display.print(value, 0);
    display.print(unit);
  } else {
    if (value > 99) {
      xval = x;
    } else {
      xval = x + charLen;
    }
    display.setCursor(xval, y);
    display.print(value, 0);
    display.drawCircle(xo, y + 2, 2, WHITE);
    display.setCursor(xunit, y);
    display.print(unit);
  }
}
