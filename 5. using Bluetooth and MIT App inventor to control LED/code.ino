int ledPin = 13;
char data;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); // Default HC-05 comm mode baud rate
}

void loop() {
  if (Serial.available()) {
    data = Serial.read();

    if (data == '1') {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    }
    else if (data == '0') {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }
  }
}
