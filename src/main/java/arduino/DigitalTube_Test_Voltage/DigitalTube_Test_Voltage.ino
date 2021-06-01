#include <DigitalTube.h>

const int VOLTAGE_PIN = A0;
DigitalTube dis(4, 5, 6);

void setup() {
  Serial.begin(9600);
  dis.begin();
  Serial.println("Ready");
}

void loop() {
  int data = analogRead(VOLTAGE_PIN); // 類比資料讀取 (0~1023)
  float voltage = data / 1024.0 * 5.0;
  int voltageInt = (int)(voltage*100);
  Serial.print("data = ");
  Serial.print(data);
  Serial.print(", voltage = ");
  Serial.print(voltage);
  Serial.print(", voltageInt = ");
  Serial.println(voltageInt);
  dis.show(voltageInt/1000, voltageInt/100%10 + 10, voltageInt/10%10, voltageInt%10);
  
  delay(2000);
}
