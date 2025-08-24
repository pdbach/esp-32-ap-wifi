// ESP32 Arduino IDE - AP mode + simple web UI
// Opens WiFi AP and serves a small web page at http://192.168.4.1
// No extra libraries required (uses WiFi.h and WebServer.h from ESP32 core)

#include <WiFi.h>
#include <WebServer.h>

// CONFIG: thay đổi nếu cần
const char* ap_ssid = "ESP32_Test_AP";
const char* ap_pass = "esp32test"; // nếu muốn open AP set thành "" (empty)
const int LED_PIN = 2; // chân LED tích hợp trên nhiều ESP32 dev boards

WebServer server(80);

// Trả về trang HTML chính
void handleRoot() {
  String html = R"rawliteral(
  <!doctype html>
  <html>
    <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width,initial-scale=1">
      <title>ESP32 AP Test</title>
      <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .card { padding: 16px; border-radius: 8px; box-shadow: 0 0 6px rgba(0,0,0,0.12); max-width:400px; }
        .status { font-weight: bold; }
        button { padding: 8px 12px; margin-top: 8px; font-size: 16px; }
      </style>
    </head>
    <body>
      <div class="card">
        <h2>ESP32 AP Test</h2>
        <p>Device IP: <span id="ip">...</span></p>
        <p>LED trạng thái: <span id="led" class="status">...</span></p>
        <button id="btnToggle">Toggle LED</button>
      </div>

      <script>
        async function updateStatus(){
          try {
            const res = await fetch('/status');
            const js = await res.json();
            document.getElementById('ip').textContent = js.ip || 'unknown';
            document.getElementById('led').textContent = js.led ? 'ON' : 'OFF';
            document.getElementById('btnToggle').textContent = js.led ? 'Turn OFF' : 'Turn ON';
          } catch(e) {
            document.getElementById('led').textContent = 'error';
            console.log(e);
          }
        }

        document.getElementById('btnToggle').addEventListener('click', async () => {
          try {
            await fetch('/toggle');

