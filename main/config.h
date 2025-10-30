#ifndef CONFIG_H
#define CONFIG_H

// --- Pines de motores ---
#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define ENB 32

// --- Pines del sensor HC-SR04 ---
#define TRIG_PIN 5
#define ECHO_PIN 18

// --- Configuración WiFi ---
#define WIFI_SSID "CrisSantiPablo"
#define WIFI_PASS "1c2s3p4IOT"

// --- Configuración MQTT ---
#define MQTT_BROKER "test.mosquitto.org"   // o la IP/URL de tu broker
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP32_CrisSantiPablo"
#define MQTT_TOPIC_MOVIMIENTO "esp32/movimiento"
#define MQTT_TOPIC_DISTANCIA "esp32/distancia"

#endif
