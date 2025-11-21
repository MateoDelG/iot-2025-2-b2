#include <Arduino.h>
#include "ArduinoJson.h"
#include "sensors.h"
#include "filters.h"

/*
JSON ESPERADO:
{
  "sensor" ; "DHT11",
  "temperature" : 23.4,
  "humidity" : 56.7,
  "state" : "OK",
  "raw_readings" : [23.4, 22.8, 24.1]
}
*/



void setup() {
Serial.begin(115200);

//-----------------------------------------SERIALIZACION JSON------------------------------------------------

// //1) Crear documento JSON
// JsonDocument doc;

// //2) lectura de sensores (simulada)
// String sensor = "DHT11";
// float temperature = 23.4;
// float humidity = 56.7;
// String state = "OK";

// float rawReadings[] = {23.4, 22.8, 24.1};

// //3) Agregar datos al documento JSON
// doc["sensor"] = sensor;
// doc["temperature"] = temperature;
// doc["humidity"] = humidity;
// doc["state"] = state;

// JsonArray readings = doc.createNestedArray("raw_readings");
// size_t n = sizeof(rawReadings) / sizeof(rawReadings[0]);
// for (size_t i = 0; i < n; ++i) {
//   readings.add(rawReadings[i]);
// }

// //4) Serializar el documento JSON a String
// String output;

// serializeJson(doc, output);

// //5) Imprimir el JSON serializado
// Serial.println(output);

// serializeJsonPretty(doc, output); //Impresion formateada en consola
// Serial.println(output);


//-----------------------------------------DESERIALIZACION JSON------------------------------------------------
// String input = R"(
// {
//   "sensor" : "DHT11",
//   "temperature" : 23.4,
//   "humidity" : 56.7,
//   "state" : "OK",
//   "raw_readings" : [23.4, 22.8, 24.1]
//   })";


// //1) Crear documento JSON
// JsonDocument doc;

// //2) Deserializar el JSON de entrada
// DeserializationError error = deserializeJson(doc, input);
// if (error) {
//   Serial.print(F("Deserialization failed: "));
//   Serial.println(error.c_str());
//   return;
// }

// //3) Extraer datos del documento JSON
// String sensor = doc["sensor"];
// float temperature = doc["temperature"];
// float humidity = doc["humidity"];
// const char* state = doc["state"];

// JsonArray readings = doc["raw_readings"].as<JsonArray>();
// //4) Imprimir los datos extraidos
// Serial.println("Extracted Data:");
// Serial.print("Sensor: ");
// Serial.println(sensor);
// Serial.print("Temperature: ");
// Serial.println(temperature);

// Serial.print("Humidity: ");
// Serial.println(humidity);

// Serial.print("State: ");
// Serial.println(state);
// Serial.print("Raw Readings: ");
// for (float reading : readings) {
//   Serial.print(reading);
//   Serial.print(" ");
// }


JsonDocument doc;

doc["device_id"] = "NODE_A01";
doc["timestamp"] = "2025-11-20T20:45:30Z";

JsonObject location = doc["location"].to<JsonObject>();
location["lat"] = 6.24737;
location["lng"] = -75.56542;
location["site"] = "Corral Principal";

JsonObject battery = doc["battery"].to<JsonObject>();
battery["voltage"] = 3.74;
battery["percentage"] = 82;

JsonObject status = doc["status"].to<JsonObject>();
status["operational"] = true;
status["mode"] = "normal";

JsonArray status_errors = status["errors"].to<JsonArray>();

JsonObject sensors = doc["sensors"].to<JsonObject>();

JsonObject sensors_temperature = sensors["temperature"].to<JsonObject>();

sensors_temperature["value"] = Sensors::DS18B20::readCelsius();
sensors_temperature["units"] = "C";
sensors_temperature["raw"] = 789;

JsonObject sensors_light = sensors["light"].to<JsonObject>();
sensors_light["value"] = 512;
sensors_light["units"] = "lx";
sensors_light["raw"] = 1023;

String output;


serializeJson(doc, output);
serializeJsonPretty(doc, output); //Impresion formateada en consola
Serial.println(output);

}

void loop()
{

}

