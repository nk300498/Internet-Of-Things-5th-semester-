#include "thingProperties.h"

const int ledPin = 5;

void setup() {
  Serial.begin(115200);
  delay(1500);
  pinMode(ledPin, OUTPUT);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
}

void onLedBrightnessChange() {
  analogWrite(ledPin, led_brightness);  // 0–255
}
