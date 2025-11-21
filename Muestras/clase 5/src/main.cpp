#include <Arduino.h>
#include "ArduinoJson.h"
#include "filters.h"
#include "sensors.h"

/*
   Ejemplo 1:
   Crear un JSON en memoria y enviarlo por el puerto serial.

   JSON resultante:
   {
     "sensor": "DHT11",
     "temp": 24.5,
     "hum": 60,
     "ok": true,
     "tags": ["lab", "prototipo"]
   }
*/

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Ejemplo 1: Serializar JSON");

  // 1. crear el documento JSON con capacidad suficiente
  JsonDocument doc;

  // 2. Llenar el objeto JSON
  doc["sensor"] = "DHT11";
  doc["temp"]   = 24.5;
  doc["hum"]    = 60;
  doc["ok"]     = true;

  // Arreglo JSON: tags
  JsonArray tags = doc["tags"].to<JsonArray>();
  tags.add("lab");
  tags.add("prototipo");

  String output;

  // 3. Serializar a la salida serial (compacto)
  Serial.println("\nJSON compacto:");
  serializeJson(doc, output);
  Serial.println(output);  // salto de línea

  // 4. Serializar en formato "bonito" (pretty print)
  Serial.println("\nJSON bonito:");
  serializeJsonPretty(doc, output);
  Serial.println(output);
}

void loop() {
  // No hacemos nada en loop; es solo demo de salida.
}



/*
   Ejemplo 2:
   Leer un JSON desde una cadena y extraer sus valores.

   JSON de entrada:
   {
     "sensor": "DHT11",
     "temp": 24.5,
     "hum": 60,
     "ok": true,
     "tags": ["lab", "prototipo"]
   }
*/
void setup() {
  Serial.begin(115200);
  delay(1000);


//   R"( ... )"
// R indica que es una raw string literal.
// Lo que está entre "( ... )" se toma literalmente.
// No se interpretan caracteres especiales como \n, \", \\, etc.
  String input = R"(
  {
    "sensor": "DHT11",
    "temp": 24.5,
    "hum": 60,
    "ok": true,
    "tags": ["lab", "prototipo"]
  }
  )";

JsonDocument doc;

DeserializationError error = deserializeJson(doc, input);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

const char* sensor = doc["sensor"]; // "DHT11"
float temp = doc["temp"]; // 24.5
int hum = doc["hum"]; // 60
bool ok = doc["ok"]; // true

//extraer elementos del arreglo
const char* tags_0 = doc["tags"][0]; // "lab"
const char* tags_1 = doc["tags"][1]; // "prototipo"


//extraer elementos del arreglo usando un bucle
  Serial.println(F("tags:"));
  JsonArray tags = doc["tags"];

  for (const char* t : tags) {
    Serial.print(F(" - "));
    Serial.println(t);
  }
  Serial.println();
}

void loop() {
  // No hacemos nada en loop; es solo demo de salida.
}