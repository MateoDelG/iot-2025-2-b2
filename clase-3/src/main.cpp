#include <Arduino.h>
#include "sensors.h"
#include "filters.h"


void setup() {
Serial.begin(115200);
// Sensors::DS18B20::begin();
Sensors::HCSR04::begin();
}

void loop() {
  float distance = Sensors::HCSR04::readDistanceCM();
  float lowPass = Filters::lowPassFilter(distance);
  float movingAvg = Filters::movingAverageFilter(distance);
  float medianVal = Filters::medianFilter(distance);
  Serial.print(">RAW:");
  Serial.println(distance);
  Serial.print(">LOW_PASS:");
  Serial.println(lowPass);
  Serial.print(">MOVING_AVG:");
  Serial.println(movingAvg);
  Serial.print(">MEDIAN:");
  Serial.println(medianVal);
}

