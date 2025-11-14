#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
#include "filters.h"
#include "sensors.h"


/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-OMrtBnZ59atggnkZXmROpTilSqM2ga";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Delga";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "Delga1213";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "temp-001";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL_1 = "temperature"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL_2 = "distance"; // Put here your Variable label to which data  will be published




Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

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

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
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
  static const int PUBLISH_FREQUENCY = 30000; // Update rate in milliseconds
  static unsigned long timer = 0;
  static const int readInterval = 2000; // Read sensor every 2 seconds
  static unsigned long readTimer = 0;

  static float temperature = Sensors::DS18B20::readCelsius();
  static float distance = Sensors::HCSR04::readDistanceCM();

  if ((millis() - readTimer) > readInterval) {
      Serial.println("Reading temperature...");
      temperature = Sensors::DS18B20::readCelsius();
      distance = Sensors::HCSR04::readDistanceCM();

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");

      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      
      readTimer = millis();
  }


  
    // put your main code here, to run repeatedly:
    if (!ubidots.connected())
    {
      ubidots.reconnect();
    }
    if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
    {
      ubidots.add(VARIABLE_LABEL_1, temperature); // Insert your variable Labels and the value to be sent
      ubidots.add(VARIABLE_LABEL_2, distance); // Insert your variable Labels and the value to be sent
      ubidots.publish(DEVICE_LABEL);
      timer = millis();
    }
    ubidots.loop();


}

