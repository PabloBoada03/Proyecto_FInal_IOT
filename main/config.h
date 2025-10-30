#ifndef CONFIG_H
#define CONFIG_H

// --- Pines de motores ---
#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 2
#define ENB 3

// --- Pines del sensor HC-SR04 ---
#define TRIG_PIN 33
#define ECHO_PIN 32

// --- Configuración WiFi ---
#define WIFI_SSID "Prieto Hotspot"
#define WIFI_PASS "J4v3r1ana"

// --- Configuración MQTT ---
#define MQTT_BROKER "broker.hivemq.com"  
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP32_CrisSantiPablo"
#define MQTT_TOPIC_MOVIMIENTO "esp32/movimiento"
#define MQTT_TOPIC_DISTANCIA "esp32/distancia"

#endif
