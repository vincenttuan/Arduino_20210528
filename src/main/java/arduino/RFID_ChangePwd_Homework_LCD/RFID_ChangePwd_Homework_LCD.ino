/*
  使用者可以自行輸入 0-255 的密碼
  例如:35
  rfid 就可以修改密碼
*/
#include <Timer.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>

#define RST_PIN 9
#define SDA_PIN 10
#define BUZEER_PIN 8
#define I2C_ADDR   0x27

RFID rfid(SDA_PIN, RST_PIN);
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// 驗證資料(block id: 11)
unsigned char oldkeyA[16] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char newkeyA[16] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int key_blockId = 11;
String inString = "";
// 1.code here
boolean isNewPasswordStatus = false; // 現在是否是在輸入新密碼的狀態
String oldPwd = " ";
String newPwd = " ";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  Serial.println("Ready");
}

void loop() {
  // 4.code here
  if(isNewPasswordStatus) { // 輸入新密碼程序
    inputNewPassword();     // 手動主控台輸入 new password
    writeNewPassword();     // rfid 卡寫入最新的 new password
  } else { // 輸入舊密碼程序
    inputOldPassword();     // 手動主控台輸入 old password
    confirmOldPassword();   // rfid 卡確認使用者所輸入的 old password
  }

  // lcd 顯示
  if(isNewPasswordStatus) {
    lcd.setCursor(0, 0);
    lcd.print("New pwd:");
    lcd.print(newPwd);
    lcd.print("  ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Old pwd:");
    lcd.print(oldPwd);
    lcd.print("  ");
  }
  
}

// 手動主控台輸入 old password
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
      oldPwd = inString;
      lcd.setCursor(0, 1);
      lcd.print("Place card:    ");
      inString = "";
    }
  }
  delay(100);
}

// 手動主控台輸入 new password
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
      newPwd = inString;
      lcd.setCursor(0, 1);
      lcd.print("Place card:    ");
      inString = "";
    }
  }
  delay(100);
}

// rfid 卡確認使用者所輸入的 old password
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
    isNewPasswordStatus = true;
  }
  //--------------------------------------------------------
  rfid.halt(); // 進入休眠
  delay(500);
}

// rfid 卡寫入最新的 new password
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
        oldPwd = " ";
        newPwd = " ";
        isNewPasswordStatus = false;
        lcd.setCursor(0, 1);
        lcd.print("Change pwd OK!");
    } else {
        Serial.println("Fail");
    }
    Serial.println("-------------------------------");
  }
  rfid.halt(); // 進入休眠
  delay(500);
}
