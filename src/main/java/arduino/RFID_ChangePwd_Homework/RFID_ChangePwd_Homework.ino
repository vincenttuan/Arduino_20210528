/*
  使用者可以自行輸入 0-255 的密碼
  例如:35
  rfid 就可以修改密碼
*/
#include <SPI.h>
#include <RFID.h>
#define RST_PIN 9
#define SDA_PIN 10
#define BUZEER_PIN 8

RFID rfid(SDA_PIN, RST_PIN);

// 驗證資料(block id: 11)
unsigned char oldkeyA[16] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char newkeyA[16] {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int key_blockId = 11;
String inString = "";
// 1.code here

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  Serial.println("Ready");
}

void loop() {
  // 4.code here
}

void inputOldPassword() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      oldkeyA[0] = inString.toInt();
      // Old password
      Serial.print("Old password: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(oldkeyA[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      inString = "";
    }
  }
  delay(100);
}

void inputNewPassword() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      newkeyA[0] = inString.toInt();
      // New password
      Serial.print("New password: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(newkeyA[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      inString = "";
    }
  }
  delay(100);
}

void confirmOldPassword() {
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
  // 授權驗證
  unsigned char status = rfid.auth(PICC_AUTHENT1A, key_blockId, oldkeyA, rfid.serNum);
  if (status == MI_OK) {
    Serial.println("Confirm old password OK !");
    Serial.println("Please input new password :");
    // 2.code here
  }
  //--------------------------------------------------------
  rfid.halt(); // 進入休眠
  delay(500);
}

void writeNewPassword() {
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
  unsigned char status = rfid.auth(PICC_AUTHENT1A, key_blockId, oldkeyA, rfid.serNum);
  if (status == MI_OK) {
    Serial.print("Change password: ");
    status = rfid.write(key_blockId, newkeyA);
    if(status == MI_OK) {
        Serial.println("OK");
        // Old password
        for(int i=0;i<5;i++) {
          Serial.print(oldkeyA[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        // New password
        for(int i=0;i<5;i++) {
          Serial.print(newkeyA[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        // 3.code here
    } else {
        Serial.println("Fail");
    }
    Serial.println("-------------------------------");
  }
  rfid.halt(); // 進入休眠
  delay(500);
}
