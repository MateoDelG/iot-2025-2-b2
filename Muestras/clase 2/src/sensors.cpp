#include "sensors.h"
#include <DallasTemperature.h>
#include <OneWire.h>


#define touchSensorPin 18
#define tempSensorPin 23

OneWire oneWire(tempSensorPin);
DallasTemperature tempSensor(&oneWire);

namespace Sensors {
namespace DS18B20 {

void begin() { tempSensor.begin(); }

float readCelsius() {
  tempSensor.requestTemperatures();
  float temp = tempSensor.getTempCByIndex(0);
//   Serial.println(temp);
return temp;
}

} // namespace DS18B20

namespace Digital {

void begin() { pinMode(touchSensorPin, INPUT); }

bool read() { return digitalRead(touchSensorPin); }
} // namespace Digital
} // namespace Sensors
