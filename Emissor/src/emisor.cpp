#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Definiciones de pines para T-Beam
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     23
#define DIO0    26

#define PIN_ANALOGIC 35  // GPIO 35 es ADC1, ideal para potenciómetros
#define LED_AZUL     4   

bool esEmissor = true; 

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_AZUL, OUTPUT);
  digitalWrite(LED_AZUL, LOW);

  // Inicializar SPI y LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(868E6)) {
    Serial.println("Error en iniciar LoRa!");
    while (1);
  }
  
  // Configuración del ADC
  analogReadResolution(12); // Resolución de 0 a 4095
  
  Serial.println(esEmissor ? "Modo EMISOR listo" : "Modo RECEPTOR listo");
}

void loop() {
  if (esEmissor) {
    // Leer el valor (será entre 0 y 4095)
    int valor = analogRead(PIN_ANALOGIC);

    digitalWrite(LED_AZUL, HIGH); 
    
    LoRa.beginPacket();
    LoRa.print(valor); // Enviamos solo el número para facilitar la recepción
    LoRa.endPacket();
    
    digitalWrite(LED_AZUL, LOW);

    Serial.print("Enviado: ");
    Serial.println(valor);
    delay(2000); 
  } else {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      digitalWrite(LED_AZUL, HIGH);
      Serial.print("Recibido: ");
      while (LoRa.available()) {
        String data = LoRa.readString();
        Serial.print(data);
      }
      Serial.println();
      digitalWrite(LED_AZUL, LOW);
    }
  }
}