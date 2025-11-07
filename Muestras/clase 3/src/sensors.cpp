#include "sensors.h"
#include <DallasTemperature.h>
#include <OneWire.h>

static const int TEMP_SENSOR_PIN = 2;

static const int ECHO_PIN  = 18;
static const int TRIG_PIN  = 5;

OneWire oneWire(TEMP_SENSOR_PIN);
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
namespace HCSR04 {
void begin() {
  pinMode(TRIG_PIN, OUTPUT); // TRIG envía un pulso de 10 µs
  pinMode(ECHO_PIN, INPUT);  // ECHO recibe el pulso devuelto
}

// -----------------------------------------------------------
//  Lectura de distancia con HC-SR04 (en cm):
//   1) TRIG: pulso de 10 µs
//   2) ECHO: medimos duración del pulso alto de retorno
//   3) Convertimos tiempo (µs) a distancia (cm) con v≈0.034 cm/µs
//   4) Si no hay eco en 30 ms o la lectura es inválida,
//      devuelve el último valor válido (para evitar saltos en la señal)
// -----------------------------------------------------------
float readDistanceCM() {
  // Variable estática: conserva su valor entre llamadas
  static float lastValid = 0.0f;

  // Asegura nivel bajo antes del pulso
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Pulso de 10 µs para disparar la medición
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout 30 ms ~ 5 m máx aprox. (evita bloqueos largos)
  unsigned long dur = pulseIn(ECHO_PIN, HIGH, 30000UL);

  // Si no se recibe eco o el valor es absurdo, usar el último válido
  if (dur == 0) {
    return lastValid;
  }

  // Distancia (cm) = (tiempo_µs * velocidad_sonido_cm/µs) / 2
  float distance = (dur * 0.034f) / 2.0f;

  // Validación adicional: descartar lecturas fuera de rango útil
  if (isnan(distance) || distance <= 2.0f || distance > 400.0f) {
    return lastValid;
  }

  // Si todo está bien, actualizar y devolver la nueva lectura
  lastValid = distance;
  return distance;
}
} // namespace HCSR04
} // namespace Sensors
