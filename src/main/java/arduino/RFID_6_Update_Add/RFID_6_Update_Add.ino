/*
  RFID 加值
  每次加值任意 0~200 元 (透過主控台將資料輸入)
  卡片最多儲值 200 元
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
int add = 0; // 每次加值金額
int maxValue = 200; // 最大加值總和
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  Serial.println("Ready");
}

void loop() {
  String inString = "";
  // 檢查是否使用者有輸入加值內容
  while(Serial.available() > 0) {
    int inChar = Serial.read();
    if(isDigit(inChar)) {
        inString += (char)inChar;
    }
    if(inChar == '\n') {
      Serial.print("Setup add: $");
      add = inString.toInt();
      Serial.println(add);
    }
  }

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
      Serial.print("add: $");
      Serial.println(add, DEC);
      // 判斷餘額是否已滿 ?
      if (balance+add <= maxValue) {
        balance += add;
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
        Serial.println("Full...");
      }
    } else {
      Serial.println("Read error");
    }
    Serial.println("-------------------------------");
  }
  rfid.halt(); // 進入休眠
  delay(500);
}
