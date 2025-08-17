#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// ==== DHT SETTINGS ====
#define DHTPIN D4       // GPIO2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ==== WIFI SETTINGS ====
const char* ssid = "Xyz";
const char* password = "aaaaaaaa";

// ==== SERVER ====
ESP8266WebServer server(80);

// Variables to store sensor readings
float temperature = 0.0;
float humidity = 0.0;

// ==== HTML PAGE ====
String htmlPage() {
  String page = "<!DOCTYPE html><html><head><title>ESP8266 Weather Station</title>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<style>";
  page += "body{font-family:Arial;text-align:center;background:linear-gradient(120deg,#89f7fe,#66a6ff);color:white;padding:20px;}";
  page += ".card{background:rgba(255,255,255,0.1);border-radius:15px;padding:20px;display:inline-block;box-shadow:0 4px 15px rgba(0,0,0,0.2);margin:10px;}";
  page += "h1{font-size:28px;}";
  page += ".value{font-size:48px;font-weight:bold;}";
  page += ".unit{font-size:24px;}";
  page += ".icon{font-size:20px;font-weight:bold;}";
  page += "</style>";
  page += "<script>setInterval(()=>{fetch('/data').then(r=>r.json()).then(d=>{document.getElementById('temp').innerText=d.temperature;document.getElementById('hum').innerText=d.humidity;});},2000);</script>";
  page += "</head><body>";
  page += "<h1>ESP8266 Weather Station</h1>";
  page += "<div class='card'><div class='icon'>Temperature</div><div class='value' id='temp'>--</div><div class='unit'>&deg;C</div></div>";
  page += "<div class='card'><div class='icon'>Humidity</div><div class='value' id='hum'>--</div><div class='unit'>%</div></div>";
  page += "</body></html>";
  return page;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlPage());
  });

  server.on("/data", HTTP_GET, []() {
    String json = "{";
    json += "\"temperature\":" + String(temperature, 1) + ",";
    json += "\"humidity\":" + String(humidity, 1);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  server.handleClient();

  static unsigned long lastRead = 0;
  if (millis() - lastRead > 2000) {
    lastRead = millis();
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.printf("Temp: %.1f°C  Hum: %.1f%%\n", temperature, humidity);
    }
  }
}
