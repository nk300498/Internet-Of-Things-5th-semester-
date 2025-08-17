#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "LED_Brightness"
#define BLYNK_AUTH_TOKEN  "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// LED pin
int ledPin = D1; // GPIO5

BLYNK_WRITE(V0)  // Runs when slider changes
{
  int brightness = param.asInt();  // 0-255
  analogWrite(ledPin, brightness);
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
}
