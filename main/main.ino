#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// --- Objetos globales ---
WiFiClient espClient;
PubSubClient client(espClient);

// --- Función para conectarse a WiFi ---
void conectarWiFi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi!");
}

// --- Función para conectar al broker MQTT ---
void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("Conectado a broker!");
    } else {
      Serial.print("Error: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// --- Función para leer el sensor (real o simulado) ---
float leerDistancia() {
  
  // return random(5, 200) / 1.0; // Simula una distancia aleatoria

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH);
  float distancia = duracion * 0.034 / 2; // Convertir a cm
  return distancia;
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  conectarWiFi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
}

// --- Loop ---
void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  float distancia = leerDistancia();
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Publicar al tema MQTT
  char mensaje[20];
  sprintf(mensaje, "%.2f", distancia);
  client.publish(MQTT_TOPIC_DISTANCIA, mensaje);

  delay(2000); // Publicar cada 2 segundos
}
