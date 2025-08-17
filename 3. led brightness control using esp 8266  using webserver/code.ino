#include <ESP8266WiFi.h>

const char* ssid = "Xyz";
const char* password = "aaaaaaaa";

int ledPin = D1;
int brightness = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // Check if request contains "value="
  if (request.indexOf("value=") != -1) {
    int pos = request.indexOf("value=") + 6;
    String val = request.substring(pos, request.indexOf(" ", pos));
    brightness = val.toInt();
    brightness = constrain(brightness, 0, 255);
    analogWrite(ledPin, brightness);
    Serial.println("Brightness set to: " + String(brightness));
  }

  // HTML page with slider
  String html = "<!DOCTYPE html><html><head><title>LED Brightness</title></head><body>";
  html += "<h1>ESP8266 LED Brightness Control</h1>";
  html += "<input type='range' min='0' max='255' value='" + String(brightness) + "' ";
  html += "oninput='fetch(`/?value=${this.value}`)'>";
  html += "<p>Brightness: " + String(brightness) + "</p>";
  html += "</body></html>";

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.print(html);
}
