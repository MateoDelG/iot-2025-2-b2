#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
#include "sensors.h"
#include "filters.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-OMrtBnZ59atggnkZXmROpTilSqM2ga";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Delga";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "Delga1213";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "control-caldera-001";   // Put here your Device label to which data  will be published
const char *TEMPERATURA = "temperatura"; // Put here your Variable label to which data  will be published
const char *NIVEL = "nivel-tanque"; // Put here your Variable label to which data  will be published

const int PUBLISH_FREQUENCY = 20000; // Update rate in milliseconds

Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {
Serial.begin(115200);
Sensors::DS18B20::begin();
Sensors::HCSR04::begin();

  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
}

void loop()
{
  static unsigned long timer = 0;

  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }

  
  if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    ubidots.add(TEMPERATURA, Sensors::DS18B20::readCelsius()); // Insert your variable Labels and the value to be sent

    float nivel = Sensors::HCSR04::readDistanceCM();
    if(nivel < 5){
      nivel = 5;
    }
    if(nivel > 40){
      nivel = 40;
    }
    nivel = map(nivel, 5, 40, 0, 100); // Map distance to tank level percentage
    
    ubidots.add(NIVEL, nivel);
    
    ubidots.publish(DEVICE_LABEL);

    timer = millis();
  }


  ubidots.loop();
}
