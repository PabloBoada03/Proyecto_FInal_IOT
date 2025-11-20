#ifndef CONFIG_H
#define CONFIG_H

// --- Pines de motores ---
#define ENA 27
#define IN1 26
#define IN2 25
#define IN3 32
#define IN4 33
#define ENB 14


// --- Pines del sensor HC-SR04 ---
#define TRIG_PIN 12
#define ECHO_PIN 13

// --- Configuración WiFi ---
#define WIFI_SSID "Prieto Hotspot"
#define WIFI_PASS "J4v3r1ana"

// --- Configuración MQTT ---
#define MQTT_BROKER "10.255.35.214"  
#define MQTT_PORT 8883
#define MQTT_CLIENT_ID "ESP32_CrisSantiPablo"
#define MQTT_TOPIC_MOVIMIENTO "esp32/movimiento"
#define MQTT_TOPIC_DISTANCIA "esp32/distancia"
#define MQTT_TOPIC_COMANDO "esp32/comando"
#endif
