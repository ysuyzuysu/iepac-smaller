#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Pins LoRa per a la TTGO T-Beam
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     23
#define DIO0    26

// Configuració WiFi
const char* ssid = "Aula 23";
const char* password = "223AuLa23";

// Configuració MQTT
const char* mqtt_server = "192.168.223.50";
const int mqtt_port = 1883;
const char* mqtt_topic = "ado/pot"; // Subtopic actualitzat

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentant connexió MQTT al subtopic 'ado/pot'...");
    // Intentem connectar
    if (client.connect("TBeam_Receptor_ADO")) {
      Serial.println("Connectat!");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentant en 5 segons...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // 1. Connectar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK. IP: " + WiFi.localIP().toString());

  // 2. Configurar servidor MQTT
  client.setServer(mqtt_server, mqtt_port);

  // 3. Configurar LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Error LoRa!");
    while (1);
  }
  Serial.println("Escoltant LoRa i preparat per publicar a 'ado/pot'...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Escoltant ràdio LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String message = "";
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    Serial.println("Rebut via LoRa: " + message);

    // Publicar al subtopic 'ado/pot'
    if (client.publish(mqtt_topic, message.c_str())) {
      Serial.println("Dades publicades a 'ado/pot' correctament.");
    } else {
      Serial.println("Error en publicar a MQTT.");
    }
  }
}
