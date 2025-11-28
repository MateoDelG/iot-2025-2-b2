#include <Arduino.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "filters.h"
#include "sensors.h"


// ---------- CONFIGURACIÓN WIFI ----------
const char* ssid     = "Delga";
const char* password = "Delga1213";

// ---------- CONFIGURACIÓN MQTT ----------
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;

const char* subTopic = "iot/test/recibir";
const char* pubTopic = "iot/test/enviar";

#define LED1 2  // Pin del LED integrado en la placa ESP32
bool ledState = false;

// ---------- CLIENTES ----------
WiFiClient espWiFiClient;
PubSubClient MQTTclient(espWiFiClient);

void callback(char* topic, byte* payload, unsigned int length);
void callbackLED(char* topic, byte* payload, unsigned int length);
void callbackJSON(char* topic, byte* payload, unsigned int length);
void reconnect();

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(LED1, OUTPUT);

  Serial.println("\nConectando a WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());

  MQTTclient.setServer(mqtt_server, mqtt_port);
  // MQTTclient.setCallback(callback);
  // MQTTclient.setCallback(callbackLED);
  MQTTclient.setCallback(callbackJSON);

}

// ---------------------------------------------------------
void loop() {
  if (!MQTTclient.connected()) {
    reconnect();
  }
  MQTTclient.loop();

  // Publicar cada 5 segundos
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    String mensaje = "Hola desde ESP32!";
    MQTTclient.publish(pubTopic, mensaje.c_str());

    Serial.print("Publicado en ");
    Serial.print(pubTopic);
    Serial.print(": ");
    Serial.println(mensaje);
  }
}

// ---------------------------------------------------------
// Callback: se ejecuta cuando llega un mensaje MQTT
// ---------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("\nMensaje recibido en: ");
  Serial.println(topic);

  Serial.print("Contenido: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void callbackLED(char* topic, byte* payload, unsigned int length) {
  Serial.print("\nMensaje recibido en: ");
  Serial.println(topic);

  // -------------------------------
  // 1) Convertir payload → String
  // -------------------------------
  String msg;
  msg.reserve(length);   // optimización opcional
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Mensaje (String): ");
  Serial.println(msg);

  // -------------------------------------
  // 2) Interpretar comandos tipo:
  //    led:on
  //    led:off
  // -------------------------------------
  if (msg.startsWith("led:")) {
    String value = msg.substring(4); // obtiene "on" o "off"

    if (value == "on") {
      digitalWrite(LED1, HIGH);
      Serial.println("→ LED encendido");
    }
    else if (value == "off") {
      digitalWrite(LED1, LOW);
      Serial.println("→ LED apagado");
    }
    else {
      Serial.println("→ Valor no reconocido para LED");
    }
  }
}

void callbackJSON(char* topic, byte* payload, unsigned int length) {
  // 1. Convertir payload a String
  String jsonStr;
  for (int i = 0; i < length; i++) jsonStr += (char)payload[i];

  Serial.print("\nJSON recibido: ");
  Serial.println(jsonStr);

  // 2. Parsear JSON
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, jsonStr);
  if (err) {
    Serial.print("Error JSON: ");
    Serial.println(err.c_str());
    return;
  }

  // 3. Leer campos
  const char* sensor = doc["sensor"];
  float temp = doc["temp"];
  int hum = doc["hum"];
  bool ok = doc["ok"];

  Serial.println(sensor);
  Serial.println(temp);
  Serial.println(hum);
  Serial.println(ok);

  if(ok) {
    digitalWrite(LED1, HIGH);  // Enciende LED si datos OK
  } else {
    digitalWrite(LED1, LOW);   // Apaga LED si datos NO OK
  }
}


// ---------------------------------------------------------
void reconnect() {
  while (!MQTTclient.connected()) {
    Serial.print("Intentando conexión MQTT... ");

     String id = "ESP32-" + String(random(0xffff), HEX);

    if (MQTTclient.connect(id.c_str())) {
      Serial.println("Conectado! id: " + id);

      MQTTclient.subscribe(subTopic);
      Serial.print("Suscrito a: ");
      Serial.println(subTopic);

    } else {
      Serial.print("Fallo. rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" Reintentando en 2 segundos...");
      delay(2000);
    }
  }
}
