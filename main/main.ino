#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// --- Objetos globales ---
WiFiClient espClient;
PubSubClient client(espClient);

// --- Función para conectarse a WiFi ---
void conectarWiFi() {
  Serial.println("🔌 Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi conectado!");
    Serial.print("📡 IP local: ");
    Serial.println(WiFi.localIP());

    // ✅ Verificar DNS del broker
    IPAddress brokerIP;
    if (WiFi.hostByName(MQTT_BROKER, brokerIP)) {
      Serial.print("DNS OK ✅ Broker: ");
      Serial.print(MQTT_BROKER);
      Serial.print(" → ");
      Serial.println(brokerIP);
    } else {
      Serial.println("❌ No se pudo resolver el nombre del broker.");
    }

  } else {
    Serial.println("\n❌ Error: no se pudo conectar al WiFi.");
  }
}

// --- Función para conectar al broker MQTT ---
void conectarMQTT() {
  Serial.print("🔗 Conectando a broker MQTT...");
  while (!client.connected()) {
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("✅ Conectado a broker MQTT!");
      client.publish("esp32/status", "ESP32 conectado correctamente ✅");
    } else {
      Serial.print("❌ Error (");
      Serial.print(client.state());
      Serial.println("). Reintentando en 2s...");
      delay(2000);
    }
  }
}

// --- Función para leer el sensor HC-SR04 ---
float leerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
  if (duracion == 0) return -1; // Sin lectura válida
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
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  float distancia = leerDistancia();
  if (distancia > 0) {
    Serial.print("📏 Distancia medida: ");
    Serial.print(distancia);
    Serial.println(" cm");

    // --- Publicar la distancia por MQTT ---
    char mensaje[20];
    snprintf(mensaje, sizeof(mensaje), "%.2f", distancia);
    client.publish(MQTT_TOPIC_DISTANCIA, mensaje);

    Serial.print("📡 Enviado al broker → ");
    Serial.print(MQTT_TOPIC_DISTANCIA);
    Serial.print(": ");
    Serial.println(mensaje);
  } else {
    Serial.println("⚠️ No se detectó eco del sensor.");
  }

  delay(2000); // Publicar cada 2 segundos
}
