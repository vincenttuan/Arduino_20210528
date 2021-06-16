#include <SPI.h>
#include <RFID.h>
#define RST_PIN 9
#define SDA_PIN 10
#define BUZEER_PIN 8

RFID rfid(SDA_PIN, RST_PIN);

// 驗證資料(block id: 11)
unsigned char    keyA[16] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char newkeyA[16] {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int key_blockId = 11;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  Serial.println("Ready");
}

void loop() {
  if (rfid.isCard() && rfid.readCardSerial()) { // 偵測到卡
    tone(BUZEER_PIN, 976, 200);
    // 讀取卡片id
    Serial.println("Card found");
    String cardId = ""; // 卡片 id
    for (int i = 0; i < 5; i++) {
      //Serial.print(rfid.serNum[i], HEX);
      //Serial.print(", ");
      cardId += String(rfid.serNum[i], HEX); // 組合前5個id內容
    }
    Serial.print("cardId: ");
    Serial.println(cardId);
    // 回傳卡片容量
    int card_size = rfid.SelectTag(rfid.serNum);
    Serial.print("Card size: ");
    Serial.print(card_size);
    Serial.println("K bits");
  }
  //--------------------------------------------------------
  // 授權驗證
  unsigned char status = rfid.auth(PICC_AUTHENT1A, key_blockId, keyA, rfid.serNum);
  if (status == MI_OK) {
    Serial.print("Change password: ");
    status = rfid.write(key_blockId, newkeyA);
    if(status == MI_OK) {
        Serial.println("OK");
        // Old password
        for(int i=0;i<5;i++) {
          Serial.print(keyA[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        // New password
        for(int i=0;i<5;i++) {
          Serial.print(newkeyA[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("Fail");
    }
    Serial.println("-------------------------------");
  }
  rfid.halt(); // 進入休眠
  delay(500);
}
