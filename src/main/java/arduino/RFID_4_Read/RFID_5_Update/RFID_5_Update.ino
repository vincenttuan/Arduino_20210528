/*
  RFID 扣款/儲值
  每次感應扣款 30 元
  扣款成功要顯示 "ok" + 最新餘額 $
  若不足要顯示 not enough
*/
#include <SPI.h>
#include <RFID.h>
#define RST_PIN 9
#define SDA_PIN 10
#define BUZEER_PIN 8

RFID rfid(SDA_PIN, RST_PIN);

// 驗證資料(block id: 11)
unsigned char keyA[16] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// 讀出資料 (block id: 9)
unsigned char readData[16];

int key_blockId = 11;
int data_blockId = 9;
int fee = 30; // 每次扣抵費用

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
  // 讀取
  // 授權驗證
  unsigned char status = rfid.auth(PICC_AUTHENT1A, key_blockId, keyA, rfid.serNum);
  if (status == MI_OK) {
    Serial.println("Auth pass");
    // 讀取資料
    status = rfid.read(data_blockId, readData); // 將資料寫入到 readData 陣列
    if (status == MI_OK) {
      int balance = readData[15];
      Serial.print("Read ok: $");
      Serial.print(balance, DEC);
      Serial.print(", 0x");
      Serial.println(balance, HEX);
      Serial.print("fee: $");
      Serial.println(fee, DEC);
      // 判斷餘額是否足夠 ?
      if (balance >= fee) {
        balance -= fee;
        // 回寫資料
        readData[15] = balance;
        status = rfid.write(data_blockId, readData);
        if (status == MI_OK) {
          Serial.print("Success last balance: $");
        } else {
          Serial.print("Fail last balance: $");  
        }
        Serial.println(balance);
      } else {
        Serial.println("Not enough");
      }
    } else {
      Serial.println("Read error");
    }
    Serial.println("-------------------------------");
  }
  rfid.halt(); // 進入休眠
  delay(500);
}
