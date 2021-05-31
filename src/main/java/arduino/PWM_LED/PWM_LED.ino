const int LED_PIN = 9; // LED 燈的接腳(9 = PIN(PWM))
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  analogWrite(LED_PIN, 0); // LED 不亮
  delay(1000);
  analogWrite(LED_PIN, 80); // LED 亮一半
  delay(1000);
  analogWrite(LED_PIN, 255); // LED 全亮
  delay(1000);    
}
