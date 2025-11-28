#ifndef SENSORS_H
#define SENSORS_H
#include <Arduino.h>

namespace Sensors {
namespace DS18B20 {
    void begin();
    float readCelsius();
}

namespace HCSR04 {
    void begin();
    float readDistanceCM();
}

}

#endif
