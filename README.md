<p align="center">
  <img width="450" height="177" alt="image" src="https://github.com/user-attachments/assets/0f8c82ab-f28b-403f-8a24-c7b41b182b02" />
</p>

<h1 align="center">Universidad de La Sabana</h1>
<h3 align="center">Facultad de Ingeniería</h3>

---

# Proyecto Final IoT — Robot 2WD con Ultrasonido y Cámara

## Descripción  
Este proyecto es un robot 2 ruedas (2WD) basado en un **ESP32** que:  

- Mide la distancia a objetos con un sensor **ultrasónico HC-SR04** para la detección de obstáculos  
- Publica los datos de distancia en un broker **MQTT**  
- Permite controlar el movimiento (adelante, atrás, izquierda, derecha, stop) desde una interfaz web  
- Muestra un stream de video a través de una cámara IP (app IP Webcam de Android)  
- Se conecta al broker MQTT de forma segura usando certificado raíz (TLS)

---

## Características principales  

- Reconocimiento de obstáculos en tiempo real  
- Publicación de telemetría (distancia / estado) en MQTT  
- Interfaz web embebida en el ESP32 para control remoto  
- Streaming de video desde un teléfono con cámara  
- Botón físico de parada de emergencia  
- Reconexión automática a WiFi y MQTT  

---

## Arquitectura del sistema  

1. El ESP32 se conecta a WiFi.  
2. Se suscribe / publica en tópicos MQTT para recibir comandos y enviar datos.  
3. Lee la distancia del sensor ultrasónico periódicamente.  
4. Publica la distancia medida en MQTT.  
5. Sirve una página web para control y monitoreo.  
6. Usa un teléfono como cámara IP para transmitir video.

---

## Instalación y configuración  

### Requisitos  
- ESP32  
- Sensor HC-SR04  
- Un teléfono Android con la app **IP Webcam**  
- Broker MQTT accesible (por ejemplo Mosquitto)  
- Red WiFi para el ESP32  

### Configuración del ESP32  
1. Clona este repositorio:  
   ```bash
   git clone https://github.com/PabloBoada03/Proyecto_FInal_IOT.git
Abre config.h y define:

WIFI_SSID y WIFI_PASS

MQTT_BROKER, MQTT_PORT, MQTT_CLIENT_ID

Los tópicos MQTT (MQTT_TOPIC_COMANDO, MQTT_TOPIC_DISTANCIA, etc.)

Pines para los motores (IN1, IN2, IN3, IN4, ENA, ENB)

Pines para el sensor ultrasónico (TRIG_PIN, ECHO_PIN)

Sube el código al ESP32 usando Arduino IDE o PlatformIO.

Uso
Enciende el ESP32 y espera a que se conecte al WiFi.

Abre tu navegador en la misma red y visita la IP del ESP32 para ver la interfaz web.

En la interfaz puedes:

Controlar el robot (adelante, atrás, izquierda, derecha, stop)

Ingresar la URL de la cámara IP Webcam para ver el video

Ajustar zoom si la cámara lo permite

Puedes suscribirte al topic MQTT de distancia para recibir lecturas en tiempo real.

Enviar comandos de movimiento desde tu propia herramienta MQTT.

API / Endpoints HTTP
El ESP32 corre un servidor web con los siguientes endpoints:

Endpoint	Método	Descripción
/	GET	Página principal con el panel de control
/move	GET	Mover el robot. Parámetros: direction, speed, duration
/distance	GET	Devuelve la última distancia medida en JSON
/status	GET	Devuelve estado del robot: distancia, estado MQTT, WiFi RSSI
/mqtt	GET	Chequea si el ESP32 está conectado al broker MQTT

Tópicos MQTT usados
Suscripción:
MQTT_TOPIC_COMANDO → recibe comandos para mover el robot.

Publicación:

MQTT_TOPIC_DISTANCIA → publica la distancia medida.

MQTT_TOPIC_MOVIMIENTO → publica cuando el robot se mueve (comando, duración, velocidad).

esp32/heartbeat → latido (“heartbeat”) con estado alive, uptime y distancia.

Librerías utilizadas
WiFi.h para conectividad WiFi

WiFiClientSecure.h para MQTT con TLS

PubSubClient.h para cliente MQTT

WebServer.h para servidor HTTP en el ESP32

ArduinoJson.h para JSON

Librerías de manejo de pines y de sensores (ultrasonido)

Uso de memoria
(Aquí puedes pegar los valores que te da Arduino IDE cuando compilas: cuánto flash y RAM usas)

text
Copiar código
Flash usada: XX%  
RAM usada: XX%  
Limitaciones actuales
No hay un dashboard remoto completo (solo la web embebida)

El streaming de video depende del teléfono, no es un ESP32-CAM

No se implementa frenado autónomo real, solo detección de obstáculos

Certificado TLS ocupa espacio significativo

Posibles mejoras
Migrar a ESP32-CAM para tener cámara integrada

Crear un dashboard web externamente (React, Node-RED, etc.)

Añadir navegación automática o frenado inteligente

Usar más sensores (encoders, IMU) para mejorar el control

Autenticación en la interfaz web / API

Contribuir
Si quieres contribuir:

Haz un fork del repositorio

Crea una rama nueva para tu feature (git checkout -b mi-feature)

Realiza tus cambios y haz commit

Abre un Pull Request describiendo lo que has hecho

Licencia
Este proyecto está bajo la Licencia MIT — ver el archivo LICENSE para más detalles.

## ✍️ Autores
**Pablo Boada Rozo, Santiago Prieto, Cristian Castañeda** — Proyecto IoT, Universidad de La Sabana  
_Octubre 2025_
