/*
  接線方式:
  GND - GND
  IN1 - D8
  VCC - 5V
*/
#define RELAY_PIN 8

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("LOW");
  delay(1000);
}
