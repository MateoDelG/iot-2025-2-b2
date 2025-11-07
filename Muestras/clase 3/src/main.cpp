#include <Arduino.h>
#include "filters.h"
#include "sensors.h"


float rawDistance();


void setup() {
  Serial.begin(115200);
  delay(200);
  
  Sensors::HCSR04::begin();

  
}

void loop() {
  float raw = rawDistance();
  float lowPass = Filters::lowPassFilter(raw);
  float movAverage = Filters::movingAverage(raw);
  float median = Filters::medianFilter(raw);

  Serial.print(">RAW:"); Serial.println(raw);
  Serial.print(">LOW_PASS:"); Serial.println(lowPass);  
  Serial.print(">MOVING_AVERAGE:"); Serial.println(movAverage);
  Serial.print(">MEDIAN_FILTER:"); Serial.println(median);
  delay(50);
}

float generateNoise(float amplitud) {
  return ((float)random(-1000, 1001) / 1000.0) * amplitud;
}

float rawDistance() {
  float distance = Sensors::HCSR04::readDistanceCM() + generateNoise(5.0);
  return distance;
}
