#include "sensor.h"
#include <DallasTemperature.h>
#include <OneWire.h>

static const int tempSensorPin = 23;

static const int ECHO_PIN = 18;
static const int TRIG_PIN = 5;

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

}

    namespace HCSR04
    {
        void begin(){
            pinMode(TRIG_PIN, OUTPUT);
            pinMode(ECHO_PIN, INPUT);
        }
        float readDistanceCM(){
            // Send trig pulse
            digitalWrite(TRIG_PIN, LOW);
            delayMicroseconds(2);
            digitalWrite(TRIG_PIN, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG_PIN, LOW);

            // Read echo pulse
            long duration = pulseIn(ECHO_PIN, HIGH);

            // Calculate distance in cm
            float distanceCM = (duration / 2.0) * 0.0343;
            return distanceCM;
        }
    }

}