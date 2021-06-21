/*
  藍牙收送程式 HC-05
  接線方式:
  RXD: D6
  TXD: D5
  GND: GND
  VCC: +5v
*/
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(5, 6); // (RX, TX)

void setup() {
  Serial.begin(9600);   // 0, 1 接腳
  BTSerial.begin(9600); // 5, 6 接腳 (9600 或 38400)
}

void loop() {
  // 檢查是否有資料透過藍芽傳進來
  if(BTSerial.available()) {
      String data = BTSerial.readString();
      Serial.print("BTSerial: ")
      Serial.println(data)
  }
  delay(200);

}
