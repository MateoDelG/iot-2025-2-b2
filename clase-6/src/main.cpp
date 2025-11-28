#include "ArduinoJson.h"
#include "filters.h"
#include "sensors.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

//broker.hivemq.com:1883

const char *ssid = "Delga";
const char *password = "Delga1213";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* pubTopic = "esp32/pub";
const char* subTopic = "esp32/sub";

WiFiClient espWiFiClient;
PubSubClient MQTTClient(espWiFiClient);

void reconnect();
void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  MQTTClient.setServer(mqtt_server, mqtt_port);
  MQTTClient.setCallback(callback);
}

void loop() { 

  if (!MQTTClient.connected()) {
    reconnect();
  }
  MQTTClient.loop();


  static unsigned long lastPublishTime = 0;
  if(millis() - lastPublishTime >= 5000) {
    
    static int counter = 0;
    counter++;
    String msg = "hola desde esp32" + String(counter);
    Serial.print("Publishing message: ");
    MQTTClient.publish(pubTopic, msg.c_str());
    Serial.println(msg);
    lastPublishTime = millis();
  }
}

void reconnect() {
  while (!MQTTClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String id = "ESP32Client" + String(random(0xffff), HEX);

    if (MQTTClient.connect(id.c_str())) {
      Serial.println("connected");

      MQTTClient.subscribe(subTopic);
      Serial.print("Subscribed to topic: ");
      Serial.println(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}