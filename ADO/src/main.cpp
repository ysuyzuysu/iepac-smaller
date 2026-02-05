// Llibreries necessàries.
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Definició de pins per a la TTGO T-Beam v1.0/1.1
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     23
#define DIO0    26

// VARIABLE CLAU: Canvia a 'true' en una placa i 'false' en l'altra
bool esEmissor = true; 

void setup() {
  Serial.begin(115200);
  
  // Configuració de pins SPI per al xip LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  // Inicialitza LoRa (868MHz és la freqüència estàndard a Europa)
  if (!LoRa.begin(868E6)) {
    Serial.println("Error en iniciar LoRa!");
    while (1);
  }
  Serial.println("LoRa iniciat correctament!");
}

void loop() {
  if (esEmissor) {
    // CODI EMISSOR
    Serial.print("Enviant paquet...");
    LoRa.beginPacket();
    LoRa.print("Smiley Face");
    LoRa.endPacket();
    delay(2000); 
  } else {
    // CODI RECEPTOR
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      Serial.print("Rebut: ");
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }
      Serial.println();
    }
  }
}
