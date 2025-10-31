#include "sensor.h"
#include <DallasTemperature.h>
#include <OneWire.h>

const int tempSensorPin = 23;

OneWire oneWire(tempSensorPin);
DallasTemperature tempSensor(&oneWire);

namespace Sensors {
namespace DS18B20 {
    
    void begin(){
        tempSensor.begin();
    }

    float readCelsius(){
        tempSensor.requestTemperatures();
        return tempSensor.getTempCByIndex(0);
    }

    int noMeMuestran(){
        return 42;
    }
}

}