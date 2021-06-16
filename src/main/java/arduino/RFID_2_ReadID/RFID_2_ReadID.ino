#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9
#define SDA_PIN 10 // SS_PIN
#define BUZEER_PIN 8

MFRC522 rfid; // 建立實體

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init(SDA_PIN, RST_PIN);
  rfid.PCD_DumpVersionToSerial(); // 讀卡設備的版本

}

void loop() {
  // 檢查是不是 rfid/nfc 卡
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    tone(BUZEER_PIN, 976, 200);
    Serial.println("Card found");
    for (int i = 0; i < 5; i++) {
      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(", ");
    }
    Serial.println();
    delay(1000);
  }
  rfid.PICC_HaltA(); // 卡片休眠
}
