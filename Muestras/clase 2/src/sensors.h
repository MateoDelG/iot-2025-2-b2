#ifndef SENSORS_H
#define SENSORS_H
#include <Arduino.h>

namespace Sensors {
namespace DS18B20 {
    void begin();
    float readCelsius();
}

namespace Digital {

    void begin();
    bool read();
}
}

#endif
