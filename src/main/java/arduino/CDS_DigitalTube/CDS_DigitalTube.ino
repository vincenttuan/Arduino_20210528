#include <DigitalTube.h>
const int VOLTAGE_PIN = A0;
const int CDS_PIN = A3;

DigitalTube dis(4, 5, 6);

void setup() {
  delay(100);
  Serial.begin(9600);
  dis.begin();
  Serial.println("Ready");
}

void loop() {
  int cdsValue = analogRead(CDS_PIN); // 0~1023
  Serial.println(cdsValue);
  dis.print(cdsValue);
  delay(500);
}
