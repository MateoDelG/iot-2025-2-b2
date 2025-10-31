#include <Arduino.h>
#include "sensors.h"


void setup() {
Serial.begin(115200);
Sensors::DS18B20::begin();
}

void loop() {
  float tempC = Sensors::DS18B20::readCelsius();
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");
  delay(2000);

}

