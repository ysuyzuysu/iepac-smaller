// Llibreries necessàries per al projecte.
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>



// Definició de pins per a la TTGO T-Beam v1.0/1.1
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     23
#define DIO0    26



// CREDENCIALS WIFI DE L'AULA
const char* ssid = "Aula 23";       
const char* password = "223AuLa23"; 

bool esEmissor = false; 

void setup() {
  // Aquesta velocitat ha de coincidir amb el monitor_speed del platformio.ini
  Serial.begin(115200);
  delay(1000); // Pausa per donar temps a la terminal a obrir-se

  Serial.println("\n--- INICIANT RECEPTOR T-BEAM ---");

  // 1. Connexió WiFi
  Serial.print("Intentant connectar a: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  // Esperem la connexió
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // Això sortirà a la terminal 
  }
  // Un cop connectat, mostrem la informació a la terminal.
  Serial.println("\n[WIFI] Connectat correctament!");
  Serial.print("[WIFI] Adreça IP: ");
  Serial.println(WiFi.localIP()); 
  Serial.println("---------------------------------");

  // 2. Configuració LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(868E6)) {
    Serial.println("[LORA] Error: No s'ha trobat el xip LoRa.");
    while (1);
  }
  Serial.println("[LORA] Escoltant missatges de l'emissor...");
}

void loop() {
  // Escoltant paquets via ràdio
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print(">>> NOU MISSATGE REBUT: ");
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.print(" (RSSI: ");
    Serial.print(LoRa.packetRssi()); // Opcional: mostra la potència del senyal
    Serial.println(" dBm)");
  }
}
