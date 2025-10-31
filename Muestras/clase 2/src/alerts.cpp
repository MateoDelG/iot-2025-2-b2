#include "alerts.h"

namespace Alerts {

    static Pins leds;

    void begin(const Pins& p) {
        leds = p;
        pinMode(leds.ok, OUTPUT);
        pinMode(leds.warning, OUTPUT);
        pinMode(leds.error, OUTPUT);
        allOff();
    }

    void allOff() {
        digitalWrite(leds.ok, LOW);
        digitalWrite(leds.warning, LOW);
        digitalWrite(leds.error, LOW);
    }

    // === Encendido y apagado individual ===
    void okOn()       { digitalWrite(leds.ok, HIGH); }
    void okOff()      { digitalWrite(leds.ok, LOW); }

    void warningOn()  { digitalWrite(leds.warning, HIGH); }
    void warningOff() { digitalWrite(leds.warning, LOW); }

    void errorOn()    { digitalWrite(leds.error, HIGH); }
    void errorOff()   { digitalWrite(leds.error, LOW); }

    // === Funciones rápidas para estados exclusivos ===
    void showOk() {
        allOff();
        okOn();
    }

    void showWarning() {
        allOff();
        warningOn();
    }

    void showError() {
        allOff();
        errorOn();
    }
}
