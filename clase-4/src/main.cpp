// #include <Arduino.h>
// #include "UbidotsEsp32Mqtt.h"
// #include "sensors.h"
// #include "filters.h"

// /****************************************
//  * Define Constants
//  ****************************************/
// const char *UBIDOTS_TOKEN = "BBFF-OMrtBnZ59atggnkZXmROpTilSqM2ga";  // Put here your Ubidots TOKEN
// const char *WIFI_SSID = "Delga";      // Put here your Wi-Fi SSID
// const char *WIFI_PASS = "Delga1213";      // Put here your Wi-Fi password
// const char *DEVICE_LABEL = "control-caldera-001";   // Put here your Device label to which data  will be published
// const char *TEMPERATURA = "temperatura"; // Put here your Variable label to which data  will be published
// const char *NIVEL = "nivel-tanque"; // Put here your Variable label to which data  will be published

// const int PUBLISH_FREQUENCY = 20000; // Update rate in milliseconds

// Ubidots ubidots(UBIDOTS_TOKEN);

// void callback(char *topic, byte *payload, unsigned int length)
// {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i = 0; i < length; i++)
//   {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();
// }


// void setup() {
// Serial.begin(115200);
// Sensors::DS18B20::begin();
// Sensors::HCSR04::begin();

//   ubidots.setDebug(true);  // uncomment this to make debug messages available
//   ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
//   ubidots.setCallback(callback);
//   ubidots.setup();
//   ubidots.reconnect();
// }

// void loop()
// {
//   static unsigned long timer = 0;

//   // put your main code here, to run repeatedly:
//   if (!ubidots.connected())
//   {
//     ubidots.reconnect();
//   }

  
//   if ((millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
//   {
//     ubidots.add(TEMPERATURA, Sensors::DS18B20::readCelsius()); // Insert your variable Labels and the value to be sent

//     float nivel = Sensors::HCSR04::readDistanceCM();
//     if(nivel < 5){
//       nivel = 5;
//     }
//     if(nivel > 40){
//       nivel = 40;
//     }
//     nivel = map(nivel, 5, 40, 0, 100); // Map distance to tank level percentage
    
//     ubidots.add(NIVEL, nivel);
//     ubidots.add(TEMPERATURA, Sensors::DS18B20::readCelsius()); 
    
//     ubidots.publish(DEVICE_LABEL);

//     timer = millis();
//   }


//   ubidots.loop();
// }

#include <Arduino.h>
#include <Wire.h>

// Cambia estos pines solo si usas una placa como ESP32/ESP8266
// En Arduino UNO/Nano/MEGA NO hace falta cambiarlos
// #define SDA_PIN 21
// #define SCL_PIN 22

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Espera a que abra el puerto serie (en placas tipo Leonardo/Micro)
  }

  Serial.println();
  Serial.println("Escáner I2C iniciado");
  Serial.println("---------------------");

  // Para ESP32 / ESP8266 podrías usar:
  // Wire.begin(SDA_PIN, SCL_PIN);
  Wire.begin();  // En Arduino clásico basta con esto
  i2cScan();
}

void loop() {
}

void i2cScan(){
   byte error, address;
  int dispositivos = 0;

  Serial.println("\nEscaneando bus I2C...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo encontrado en 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();

      dispositivos++;
    } else if (error == 4) {
      Serial.print("Error desconocido en dirección 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (dispositivos == 0) {
    Serial.println("No se encontraron dispositivos I2C :(");
  } else {
    Serial.print("Total de dispositivos encontrados: ");
    Serial.println(dispositivos);
  }

  Serial.println("Escaneo terminado. Repitiendo en 3 segundos...");
  delay(3000);
}