#ifndef ALERTS_H
#define ALERTS_H
#include <Arduino.h>

namespace Alerts {

    struct Pins {
        int ok;
        int warning;
        int error;
    };

    void begin(const Pins& p);

    // Control general
    void allOff();

    // Control individual
    void okOn();
    void okOff();

    void warningOn();
    void warningOff();

    void errorOn();
    void errorOff();

    // Funciones exclusivas (mantienen compatibilidad con versiones previas)
    void showOk();
    void showWarning();
    void showError();
}

#endif
