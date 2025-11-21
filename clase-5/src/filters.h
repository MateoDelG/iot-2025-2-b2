#ifndef FILTERS_H
#define FILTERS_H
#include <Arduino.h>

namespace Filters {

    float lowPassFilter(float input);

    float movingAverageFilter(float input);

    float medianFilter(float input);

}

#endif