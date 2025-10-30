# 🚀 Proyecto ESP32 con Sensor HC-SR04 y MQTT

Este proyecto implementa la lectura de distancia con el **sensor ultrasónico HC-SR04** conectado a una **ESP32**, publicando los valores obtenidos en un **tema MQTT**.  
El código también define los pines de motores y la configuración WiFi mediante constantes en un archivo `config.h`.

## 📁 Estructura del Proyecto
/main/
├── main.ino → Código principal
└── config.h → Configuración de pines, WiFi y MQTT

## ⚙️ Configuración
- **WiFi:** se define con `WIFI_SSID` y `WIFI_PASS`
- **Broker MQTT:** `test.mosquitto.org` (puede cambiarse)
- **Temas MQTT:**
  - `esp32/distancia` → publica lecturas del sensor  
  - `esp32/movimiento` → reservado para control de motores

## 🔌 Conexión del HC-SR04
| Pin HC-SR04 | ESP32 | Nota |
|--------------|--------|------|
| VCC | 5V | Alimentación |
| GND | GND | Tierra |
| TRIG | GPIO 5 | Señal de disparo |
| ECHO | GPIO 18 | ⚠️ usar divisor de voltaje (1kΩ / 2kΩ) |

## 🧠 Función principal
- Conecta a WiFi y MQTT.
- Lee el sensor periódicamente (o simula lectura).
- Publica la distancia cada 2 segundos en el tema MQTT.

## ✍️ Autores
**Pablo Boada Rozo, Santiago Prieto, Cristian Castañeda** — Proyecto IoT, Universidad de La Sabana  
_Octubre 2025_
