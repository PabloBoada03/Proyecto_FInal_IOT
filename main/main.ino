codigo final esp32 prueba 




#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include "config.h"

// ---------------- Objetos globales ----------------
WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);

// ---------------- Variables globales ----------------
int velocidad = 200;
volatile bool stopFlag = false;
float ultimaDistancia = 0;
const int STOP_PIN = 4;  // Botón físico de emergencia

// ---------------- Interrupción física ----------------
void IRAM_ATTR stopInterrupt() {
  stopFlag = true;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------------- Función para leer distancia ----------------
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

// ---------------- Control del carro ----------------
void moverMotores(int m1a, int m1b, int m2a, int m2b, int duty) {
  if (stopFlag) return;
  digitalWrite(IN1, m1a);
  digitalWrite(IN2, m1b);
  digitalWrite(IN3, m2a);
  digitalWrite(IN4, m2b);
  analogWrite(ENA, duty);
  analogWrite(ENB, duty);
}

void adelante() { moverMotores(LOW, HIGH, LOW, HIGH, velocidad); }
void atras() { moverMotores(HIGH, LOW, HIGH, LOW, velocidad); }
void izquierda() { moverMotores(HIGH, LOW, LOW, HIGH, velocidad); }
void derecha() { moverMotores(LOW, HIGH, HIGH, LOW, velocidad); }
void parar() {
  moverMotores(LOW, LOW, LOW, LOW, 0);
  stopFlag = false;
}

// ---------------- HTML de la interfaz ----------------
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Control del Carro ESP32</title>
<style>
  body { font-family: Arial; text-align: center; background: #f0f0f0; padding: 20px; }
  .btn { width: 80px; height: 80px; border: none; border-radius: 50%;
         font-size: 30px; color: white; margin: 10px; cursor: pointer; }
  .up { background: #2ecc71; } .down { background: #e74c3c; }
  .left, .right { background: #f1c40f; } .stop { background: #c0392b; width: 90px; height: 90px; font-size: 20px; }
  .controls { display: flex; flex-direction: column; align-items: center; }
  .row { display: flex; justify-content: center; align-items: center; }
  .slider-container { width: 80%; margin: 20px auto; }
  input[type="range"] { width: 100%; }
  #status, #mqttStatus { margin-top: 15px; }
  #mqttStatus.online { color: green; font-weight: bold; }
  #mqttStatus.offline { color: red; font-weight: bold; }
  
  .distance-container { 
    width: 80%; 
    margin: 20px auto; 
    padding: 15px; 
    background: white; 
    border-radius: 10px; 
    box-shadow: 0 2px 5px rgba(0,0,0,0.1); 
  }
  .distance-bar { 
    width: 100%; 
    height: 30px; 
    background: #e0e0e0; 
    border-radius: 15px; 
    overflow: hidden; 
    position: relative; 
  }
  .distance-fill { 
    height: 100%; 
    transition: width 0.3s, background-color 0.3s; 
    border-radius: 15px; 
  }
  .distance-text { 
    font-size: 24px; 
    font-weight: bold; 
    margin: 10px 0; 
  }
  .distance-warning { 
    color: #e74c3c; 
    font-weight: bold; 
    display: none; 
  }

  .camera-container {
    width: 95%;
    max-width: 1200px;
    margin: 20px auto;
    padding: 20px;
    background: white;
    border-radius: 10px;
    box-shadow: 0 2px 5px rgba(0,0,0,0.1);
  }
  .camera-input-group {
    display: flex;
    gap: 10px;
    margin-bottom: 15px;
    flex-wrap: wrap;
    justify-content: center;
  }
  .camera-input-group input {
    flex: 1;
    min-width: 200px;
    padding: 10px;
    border: 2px solid #3498db;
    border-radius: 5px;
    font-size: 14px;
  }
  .camera-input-group button {
    padding: 10px 20px;
    background: #3498db;
    color: white;
    border: none;
    border-radius: 5px;
    cursor: pointer;
    font-size: 14px;
    font-weight: bold;
  }
  .camera-input-group button:hover {
    background: #2980b9;
  }
  .camera-input-group button.clear-btn {
    background: #e74c3c;
  }
  .camera-input-group button.clear-btn:hover {
    background: #c0392b;
  }
  #cameraFrame {
    width: 100%;
    height: 700px;
    border: 2px solid #ddd;
    border-radius: 5px;
    display: none;
    background: #000;
  }
  #cameraImg {
    width: 100%;
    max-height: 700px;
    border: 2px solid #ddd;
    border-radius: 5px;
    display: none;
    background: #000;
    object-fit: contain;
  }
  .camera-help {
    font-size: 12px;
    color: #7f8c8d;
    margin-top: 10px;
  }
  .camera-status {
    margin: 10px 0;
    font-weight: bold;
  }
  .camera-status.active {
    color: #2ecc71;
  }
  .camera-status.inactive {
    color: #95a5a6;
  }
  .camera-controls {
    display: flex;
    gap: 10px;
    margin: 15px 0;
    flex-wrap: wrap;
    justify-content: center;
    align-items: center;
  }
  .zoom-slider {
    flex: 1;
    min-width: 200px;
    max-width: 400px;
  }
  .zoom-btn {
    padding: 10px 15px;
    background: #9b59b6;
    color: white;
    border: none;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
    font-weight: bold;
  }
  .zoom-btn:hover {
    background: #8e44ad;
  }
  .zoom-info {
    font-weight: bold;
    color: #34495e;
    min-width: 80px;
  }
</style>
</head>
<body>
<h2>🚗 Control del Carro ESP32</h2>
<p>IP del ESP32: <span id="esp_ip"></span></p>
<div id="mqttStatus" class="offline">MQTT: Desconectado ❌</div>

<div class="camera-container">
  <h3>📹 Cámara IP Webcam</h3>
  <div id="cameraStatus" class="camera-status inactive">Cámara: No conectada</div>
  <div class="camera-input-group">
    <input type="text" id="cameraUrl" placeholder="Ingresa la URL de IP Webcam (ej: http://192.168.1.100:8080/video)">
    <button onclick="connectCamera()">Conectar</button>
    <button class="clear-btn" onclick="disconnectCamera()">Desconectar</button>
  </div>
  
  <div class="camera-controls">
    <button class="zoom-btn" onclick="zoomOut()">🔍-</button>
    <input type="range" class="zoom-slider" id="zoomSlider" min="0" max="100" value="0" oninput="setZoom(this.value)">
    <button class="zoom-btn" onclick="zoomIn()">🔍+</button>
    <span class="zoom-info" id="zoomValue">Zoom: 0%</span>
  </div>
  
  <iframe id="cameraFrame"></iframe>
  <img id="cameraImg" alt="Stream de cámara">
  <div class="camera-help">
    💡 Formato típico de IP Webcam: http://TU_IP:8080/video<br>
    📹 Alternativas: /videofeed o /shot.jpg<br>
    Asegúrate de que tu teléfono y el ESP32 estén en la misma red WiFi
  </div>
</div>

<div class="distance-container">
  <h3>📏 Sensor de Distancia</h3>
  <div class="distance-text" id="distanceValue">-- cm</div>
  <div class="distance-bar">
    <div class="distance-fill" id="distanceBar"></div>
  </div>
  <p class="distance-warning" id="distanceWarning">⚠️ ¡Objeto muy cerca!</p>
</div>

<div class="controls">
  <div class="row"><button class="btn up" onclick="sendCommand('forward')">↑</button></div>
  <div class="row">
    <button class="btn left" onclick="sendCommand('left')">←</button>
    <button class="btn stop" onclick="sendCommand('stop')">STOP</button>
    <button class="btn right" onclick="sendCommand('right')">→</button>
  </div>
  <div class="row"><button class="btn down" onclick="sendCommand('backward')">↓</button></div>
</div>

<div class="slider-container">
  <label>Velocidad: </label>
  <input type="range" id="speed" min="0" max="255" value="200">
</div>

<div class="slider-container">
  <label>Duración (ms): </label>
  <input type="range" id="duration" min="100" max="5000" value="1000" step="100">
  <p><span id="durValue">1000</span> ms</p>
</div>

<p id="status"></p>

<script>
const esp32_ip = window.location.hostname;
document.getElementById("esp_ip").textContent = esp32_ip;

let cameraBaseUrl = "";

function connectCamera() {
  const url = document.getElementById("cameraUrl").value.trim();
  if (!url) {
    alert("⚠️ Por favor ingresa una URL válida");
    return;
  }
  
  cameraBaseUrl = url.split('/video')[0].split('/videofeed')[0].split('/shot')[0];
  
  const iframe = document.getElementById("cameraFrame");
  const img = document.getElementById("cameraImg");
  const status = document.getElementById("cameraStatus");
  
  if (url.includes('/video') || url.includes('/videofeed')) {
    iframe.src = url;
    iframe.style.display = "block";
    img.style.display = "none";
  } else if (url.includes('/shot.jpg') || url.endsWith('.jpg')) {
    img.src = url + "?t=" + Date.now();
    img.style.display = "block";
    iframe.style.display = "none";
    
    if (window.cameraInterval) clearInterval(window.cameraInterval);
    window.cameraInterval = setInterval(() => {
      img.src = url + "?t=" + Date.now();
    }, 100);
  } else {
    iframe.src = url;
    iframe.style.display = "block";
    img.style.display = "none";
  }
  
  status.textContent = "Cámara: Conectada ✅";
  status.className = "camera-status active";
  localStorage.setItem("cameraUrl", url);
  console.log("📹 Conectando cámara:", url);
}

function setZoom(value) {
  if (!cameraBaseUrl) {
    alert("⚠️ Primero conecta la cámara");
    return;
  }
  
  document.getElementById("zoomValue").textContent = `Zoom: ${value}%`;
  const zoomUrl = `${cameraBaseUrl}/settings/ptz?set=zoom&zoom=${value}`;
  
  fetch(zoomUrl)
    .then(res => {
      if (res.ok) console.log(`✅ Zoom ajustado a ${value}%`);
      else console.log(`⚠️ Error al ajustar zoom`);
    })
    .catch(err => console.log("❌ Error de conexión:", err));
}

function zoomIn() {
  const slider = document.getElementById("zoomSlider");
  let newValue = parseInt(slider.value) + 10;
  if (newValue > 100) newValue = 100;
  slider.value = newValue;
  setZoom(newValue);
}

function zoomOut() {
  const slider = document.getElementById("zoomSlider");
  let newValue = parseInt(slider.value) - 10;
  if (newValue < 0) newValue = 0;
  slider.value = newValue;
  setZoom(newValue);
}

function disconnectCamera() {
  const iframe = document.getElementById("cameraFrame");
  const img = document.getElementById("cameraImg");
  const status = document.getElementById("cameraStatus");
  
  iframe.src = "";
  iframe.style.display = "none";
  img.src = "";
  img.style.display = "none";
  status.textContent = "Cámara: No conectada";
  status.className = "camera-status inactive";
  
  if (window.cameraInterval) {
    clearInterval(window.cameraInterval);
    window.cameraInterval = null;
  }
  
  document.getElementById("zoomSlider").value = 0;
  document.getElementById("zoomValue").textContent = "Zoom: 0%";
  cameraBaseUrl = "";
  console.log("📹 Cámara desconectada");
}

window.onload = function() {
  const savedUrl = localStorage.getItem("cameraUrl");
  if (savedUrl) document.getElementById("cameraUrl").value = savedUrl;
};

async function sendCommand(direction) {
  const speed = document.getElementById("speed").value;
  const duration = document.getElementById("duration").value;
  const url = `http://${esp32_ip}/move?direction=${direction}&speed=${speed}&duration=${duration}`;
  document.getElementById("status").textContent = "Enviando...";
  try {
    const res = await fetch(url);
    const data = await res.json();
    document.getElementById("status").textContent = "✅ " + data.cmd;
  } catch { document.getElementById("status").textContent = "❌ Error"; }
}

document.getElementById("duration").oninput = function() {
  document.getElementById("durValue").textContent = this.value;
};

async function checkMQTT() {
  try {
    const res = await fetch(`http://${esp32_ip}/mqtt`);
    const data = await res.json();
    const el = document.getElementById("mqttStatus");
    if (data.online) { el.textContent = "MQTT: Conectado ✅"; el.className = "online"; }
    else { el.textContent = "MQTT: Desconectado ❌"; el.className = "offline"; }
  } catch {}
}

async function updateDistance() {
  try {
    const res = await fetch(`http://${esp32_ip}/distance`);
    const data = await res.json();
    const distancia = data.distance;
    
    document.getElementById("distanceValue").textContent = distancia.toFixed(1) + " cm";
    
    const maxDist = 200;
    let percentage = (distancia / maxDist) * 100;
    if (percentage > 100) percentage = 100;
    
    const bar = document.getElementById("distanceBar");
    bar.style.width = percentage + "%";
    
    if (distancia < 10) {
      bar.style.background = "#e74c3c";
      document.getElementById("distanceWarning").style.display = "block";
    } else if (distancia < 30) {
      bar.style.background = "#f39c12";
      document.getElementById("distanceWarning").style.display = "none";
    } else {
      bar.style.background = "#2ecc71";
      document.getElementById("distanceWarning").style.display = "none";
    }
  } catch {}
}

setInterval(checkMQTT, 2000);
setInterval(updateDistance, 500);
</script>
</body>
</html>
)rawliteral";

// ---------------- Endpoints ----------------
void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", MAIN_page); 
}

void handleMove() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if (!server.hasArg("direction")) {
    server.send(400, "application/json", "{\"error\":\"direction required\"}");
    return;
  }

  stopFlag = false;
  String direction = server.arg("direction");
  int speed = server.hasArg("speed") ? server.arg("speed").toInt() : velocidad;
  int duration = server.hasArg("duration") ? server.arg("duration").toInt() : 1000;

  velocidad = constrain(speed, 0, 255);
  duration = constrain(duration, 100, 5000);

  if (direction == "forward") adelante();
  else if (direction == "backward") atras();
  else if (direction == "left") izquierda();
  else if (direction == "right") derecha();
  else parar();

  String payload = "{\"direction\":\"" + direction + "\",\"speed\":" + String(speed) + ",\"distance\":" + String(ultimaDistancia) + "}";
  if (client.connected()) {
    client.publish(MQTT_TOPIC_MOVIMIENTO, payload.c_str());
  }

  server.send(200, "application/json", "{\"status\":\"ok\",\"cmd\":\"" + direction + "\"}");

  unsigned long start = millis();
  while (millis() - start < duration && !stopFlag) {
    delay(10);
  }
  parar();
}

void handleMQTT() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  bool online = client.connected();
  server.send(200, "application/json", "{\"online\":" + String(online ? "true" : "false") + "}");
}

void handleDistance() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"distance\":" + String(ultimaDistancia) + "}");
}

void handleStatus() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"status\":\"running\",\"distance\":" + String(ultimaDistancia) + "}");
}

// ---------------- Conectar WiFi ----------------
void conectarWiFi() {
  // Si ya está conectado, no hacer nada
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }
  
  Serial.println("🔌 Conectando a WiFi...");
  WiFi.disconnect(true);  // Desconectar antes de reconectar
  delay(1000);
  
  WiFi.mode(WIFI_STA);  // Modo estación
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
  } else {
    Serial.println("\n❌ Error: no se pudo conectar al WiFi.");
  }
}

// ---------------- Conectar MQTT ----------------
void conectarMQTT() {
  if (!client.connected()) {
    Serial.print("🔗 Conectando a broker MQTT...");
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("✅ Conectado a broker MQTT!");
      client.publish("esp32/status", "ESP32 conectado correctamente ✅");
    } else {
      Serial.print("❌ Error (");
      Serial.print(client.state());
      Serial.println(")");
    }
  }
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT);
  
  pinMode(STOP_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOP_PIN), stopInterrupt, FALLING);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  conectarWiFi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
  conectarMQTT();

  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/mqtt", handleMQTT);
  server.on("/distance", handleDistance);
  server.on("/status", handleStatus);
  server.begin();
  
  Serial.println("🌐 Servidor HTTP iniciado");
  Serial.print("🔗 Accede desde: http://");
  Serial.println(WiFi.localIP());
}

// ---------------- Loop ----------------
unsigned long ultimaLectura = 0;
unsigned long ultimaReconexion = 0;
const unsigned long intervaloLectura = 500;
const unsigned long intervaloReconexion = 30000; // Intentar reconectar cada 30 segundos

void loop() {
  server.handleClient();

  // Reconectar WiFi si es necesario (cada 30 segundos)
  if (WiFi.status() != WL_CONNECTED && millis() - ultimaReconexion >= intervaloReconexion) {
    ultimaReconexion = millis();
    conectarWiFi();
  }

  // Reconectar MQTT si es necesario
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    conectarMQTT();
  }
  
  if (client.connected()) {
    client.loop();
  }

  if (millis() - ultimaLectura >= intervaloLectura) {
    ultimaLectura = millis();
    
    float distancia = leerDistancia();
    if (distancia > 0) {
      ultimaDistancia = distancia;
      
      Serial.print("📏 Distancia: ");
      Serial.print(distancia);
      Serial.println(" cm");

      char mensaje[20];
      snprintf(mensaje, sizeof(mensaje), "%.2f", distancia);
      if (client.connected()) {
        client.publish(MQTT_TOPIC_DISTANCIA, mensaje);
      }
    } else {
      Serial.println("⚠️ Sensor sin lectura válida");
    }
  }
}
