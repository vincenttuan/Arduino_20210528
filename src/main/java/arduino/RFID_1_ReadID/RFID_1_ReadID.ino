#include <SPI.h>
#include <RFID.h>
#define RST_PIN 9
#define SDA_PIN 10
#define BUZEER_PIN 8

/*
RFID 接線
SDA: 10
SCK: 13
MOSI: 11
MISO: 12
IRQ: 
GND: GND
RST: 9
3.3V: 3.3V
*/

RFID rfid(SDA_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  Serial.println("Ready");
}

void loop() {
  if(rfid.isCard() && rfid.readCardSerial()) { // 偵測到卡
      tone(BUZEER_PIN, 976, 200);
      Serial.println("Card found");
      String cardId = ""; // 卡片 id
      for(int i=0;i<5;i++) {
        //Serial.print(rfid.serNum[i], HEX);
        //Serial.print(", ");
        cardId += String(rfid.serNum[i], HEX); // 組合前5個id內容
      }
      Serial.print("cardId: ");
      Serial.println(cardId);
      Serial.println();
      delay(1000);
  }

  rfid.halt(); // 進入休眠
}
