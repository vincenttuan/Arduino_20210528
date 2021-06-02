/*
接線方式：
SIG: A3
VCC: 5V
GND: GND
*/
const int CDS_PIN = A3;

void setup() {
  delay(100);
  Serial.begin(9600);
}

void loop() {
  int cdsValue = analogRead(CDS_PIN); // 0~1023
  Serial.println(cdsValue);
  delay(500);
}
