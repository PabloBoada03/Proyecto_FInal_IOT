<p align="center">
  <img width="450" height="177" alt="image" src="https://github.com/user-attachments/assets/0f8c82ab-f28b-403f-8a24-c7b41b182b02" />
</p>

<h1 align="center">Universidad de La Sabana</h1>
<h3 align="center">Facultad de Ingeniería</h3>

---

## 🚀 Proyecto ESP32 con Sensor HC-SR04 y MQTT

### 📘 Descripción del Proyecto
Este proyecto implementa la **lectura de distancia** utilizando el **sensor ultrasónico HC-SR04**, conectado a una **placa ESP32**.  
Los valores obtenidos son publicados en un **tema MQTT**, permitiendo el monitoreo remoto mediante un broker.  
Además, el sistema incluye la definición de pines de motores y la configuración WiFi centralizada en un archivo `config.h`.

---

### 👨‍💻 Integrantes
- Cristian Manuel Castañeda Gutiérrez  
- Pablo Bohada  
- Santiago Prieto Trujillo  

---

<p align="center">
  <img width="425" height="425" alt="image" src="https://github.com/user-attachments/assets/0bf9401d-c987-48c3-98f7-021de7437280" />
</p>

<p align="center">
  <b>Universidad de La Sabana</b><br>
  Estudiantes de la Facultad de Ingeniería<br>
  Chía, Cundinamarca — 2025
</p>

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
