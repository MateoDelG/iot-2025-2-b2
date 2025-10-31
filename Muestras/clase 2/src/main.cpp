#include "alerts.h"
#include "sensors.h"
#include <Arduino.h>


// =====================
// Configuración de pines (ajusta a tu hardware)
// =====================
const int PIN_LED_OK = 15;     // Verde  → temperatura OK
const int PIN_LED_WARNING = 2; // Amarillo → estado del sensor digital
const int PIN_LED_ERROR = 4;   // Rojo   → temperatura alta


// Prototipos
void handleTemperature();
void handleDigital();

void setup() {
  Serial.begin(115200);
  delay(200);

  Alerts::Pins leds = {PIN_LED_OK, PIN_LED_WARNING, PIN_LED_ERROR};
  Alerts::begin(leds); // deja todo apagado

  Sensors::DS18B20::begin();
  Sensors::Digital::begin();

}

void loop() {
  handleDigital(); // siempre refleja el estado del digital en el LED amarillo
  handleTemperature(); // actualiza los LEDs verde/rojo según temperatura
}

void handleDigital() {
  static const unsigned long DIGI_INTERVAL_MS = 50; // lectura digital rápida
  static unsigned long lastCheck = 0;
  static bool lastState = false; // Guarda el estado anterior del pin
  static bool ledState = false;  // Estado actual del LED amarillo (toggle)

  const unsigned long now = millis();
  if (now - lastCheck < DIGI_INTERVAL_MS)
    return;
  lastCheck = now;

  const bool currentState =
      Sensors::Digital::read(); // HIGH activo (ajusta si usas PULLUP)

  // Detecta cambio de BAJO → ALTO (flanco ascendente)
  if (currentState && !lastState) {
    ledState = !ledState; // Cambia el estado del LED
    if (ledState) {
      Alerts::warningOn();
    } else {
      Alerts::warningOff();
    }

    Serial.print(F("[DIGITAL] Flanco BAJO→ALTO → LED "));
    Serial.println(ledState ? F("ENCENDIDO") : F("APAGADO"));
  }

  lastState = currentState; // Actualiza el estado previo
}

void handleTemperature() {
  static const unsigned long TEMP_INTERVAL_MS = 2000; // lectura DS18B20
  static const float TEMP_WARN_C = 25.0f;             // umbral inferior
  static const float TEMP_ERR_C = 30.0f;              // umbral superior

  static unsigned long lastRead = 0;
  const unsigned long now = millis();

  if (now - lastRead < TEMP_INTERVAL_MS)
    return;
  lastRead = now;

  const float t = Sensors::DS18B20::readCelsius();
  Serial.print(F("[TEMP] "));
  Serial.print(t);
  Serial.println(F(" °C"));


  Alerts::okOff();
  Alerts::errorOff();

  if (t < TEMP_WARN_C) {
    Alerts::okOn(); // Temperatura normal → verde
  } else if (t >= TEMP_ERR_C) {
    Alerts::errorOn(); // Temperatura alta → rojo
  }
  // En el rango intermedio (TEMP_WARN_C .. TEMP_ERR_C) quedan ambos OFF
}
