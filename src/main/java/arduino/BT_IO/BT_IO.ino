/*
  藍牙收送程式 HC-05
  接線方式:
  RXD: D6
  TXD: D5
  GND: GND
  VCC: +5v
*/
#include <SoftwareSerial.h>
#define BUTTON_PIN 7

SoftwareSerial BTSerial(5, 6); // (RX, TX)

void setup() {
  Serial.begin(9600);   // 0, 1 接腳
  BTSerial.begin(9600); // 5, 6 接腳 (9600 或 38400)
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // 檢查是否有資料透過藍芽傳進來
  if(BTSerial.available()) {
      String data = BTSerial.readString(); // 讀取藍芽傳來的資料
      Serial.print("BTSerial Receive: ");
      Serial.println(data);
  }
  // 透過藍芽將資料送給 App
  if(digitalRead(BUTTON_PIN) == LOW) {
    int n = random(0, 100);
    BTSerial.println(n); // 將資料透過藍芽送出
    Serial.print("Serial Send: ");
    Serial.println(n);
    delay(300);
  }
  delay(200);

}
