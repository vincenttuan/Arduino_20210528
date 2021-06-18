int num = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  num = random(0, 100); // 0~99
  Serial.println(num);
  delay(500);
}
