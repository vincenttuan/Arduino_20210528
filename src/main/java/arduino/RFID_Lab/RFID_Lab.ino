/*
  RFID, LCD, Button, Buzeer
  LCD 畫面
  (M:0 查看餘額) 按下 button
  +----------------+
  |           M:0  |
  |Bal:$0          |
  +----------------+

  (M:0 查看餘額) 刷卡
  +----------------+
  |A1B2C3D4E5 M:0  |
  |Bal:$180        |
  +----------------+

  (M:1 消費) 按下 button
  +----------------+
  |           M:1  |
  |Msg:      Fee:30| (30元固定)
  +----------------+

  (M:1 消費) 刷卡
  +----------------+
  |A1B2C3D4E5 M:1  |
  |Msg:OK    Fee:30| Msg:OK/Fail
  +----------------+

  (M:2 儲值) 按下 button
  +----------------+
  |           M:2  |
  |Msg:     Add:0  | (0 初始資料或前一次的資料)
  +----------------+

  (M:2 儲值) 從主控台輸入數字
  +----------------+
  |           M:2  |
  |Msg:     Add:50 | (不超過200)
  +----------------+

  (M:3 儲值) 刷卡
  +----------------+
  |A1B2C3D4E5 M:2  |
  |Msg:OK   Add:50 | Msg:OK/Fail
  +----------------+
*/
#include <Timer.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <RFID.h>
#define RST_PIN 9
#define SDA_PIN 10
#define BUZEER_PIN 8
#define BUTTON_PIN 7
#define I2C_ADDR   0x27

RFID rfid(SDA_PIN, RST_PIN);
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Timer timer1, timer2, timer3, timer4;

// 驗證資料(block id: 11)
unsigned char keyA[16] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// 讀出資料 (block id: 9)
unsigned char readData[16];

int key_blockId = 11;
int data_blockId = 9;

boolean isDetectCard = false;
int fee = 30; // 每次消費
int add = 0; // 每次加值金額
int maxValue = 200; // 最大加值總和
int mode = 0;

// lcd default value
String cardId = ""; // 卡片 id
int balance = 0;
String msg = "";

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);

  SPI.begin();
  rfid.init();

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();

  // 配置排程
  timer1.every(100, listenerButton);
  timer2.every(500, playLCD);
  timer3.every(200, changeAdd);
  timer4.every(200, detectRFID);
}

void detectRFID() {
  if (rfid.isCard() && rfid.readCardSerial()) { // 偵測到卡
    clearLCDDefaultValue();
    tone(BUZEER_PIN, 976, 200);
    // 讀取卡片id
    Serial.println("Card found");
    cardId = ""; // 卡片 id
    for (int i = 0; i < 5; i++) {
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

    switch (mode % 3) {
      case 0:
        if (status == MI_OK) {
          balance = readData[15];
          Serial.print("balance: $");
          Serial.println(balance);
        } else {
          Serial.println("Read error");
        }
        break;
      case 1:
        if (status == MI_OK) {
          balance = readData[15];
          // 判斷餘額是否足夠 ?
          if (balance >= fee) {
            balance -= fee;
            // 回寫資料
            readData[15] = balance;
            status = rfid.write(data_blockId, readData);
            if (status == MI_OK) {
              msg = "OK";
              Serial.print("Success last balance: $");
            } else {
              msg = "FAIL";
              Serial.print("Fail last balance: $");
            }
            Serial.println(balance);
          } else {
            msg = "NOT";
            Serial.println("Not enough");
          }
        } else {
          msg = "ERROR";
          Serial.println("Read error");
        }
      case 2:
        if (status == MI_OK) {
          balance = readData[15];
          // 餘額是否超過
          if (balance+add <= maxValue) {
            balance += add;
            // 回寫資料
            readData[15] = balance;
            status = rfid.write(data_blockId, readData);
            if (status == MI_OK) {
              msg = "OK";
              Serial.print("Success last balance: $");
            } else {
              msg = "FAIL";
              Serial.print("Fail last balance: $");
            }
            Serial.println(balance);
          } else {
            msg = "NOT";
            Serial.println("Not enough");
          }
        } else {
          msg = "ERROR";
          Serial.println("Read error");
        }
        break;

    }
    Serial.println("-------------------------------");
  }
  rfid.halt(); // 進入休眠
  delay(500);
}
void changeAdd() {
  String inString = "";
  // 檢查是否使用者有輸入加值內容
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
    }
    if (inChar == '\n') {
      Serial.print("Setup add: $");
      add = inString.toInt();
      Serial.println(add);
    }
  }
}
void playLCD() {
  switch (mode % 3) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print(cardId);
      lcd.setCursor(11, 0);
      lcd.print("M:0");
      lcd.setCursor(0, 1);
      lcd.print("Bal:$           ");
      lcd.setCursor(5, 1);
      lcd.print(balance);
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print(cardId);
      lcd.setCursor(11, 0);
      lcd.print("M:1");
      lcd.setCursor(0, 1);
      lcd.print("Msg:     Fee:   ");
      lcd.setCursor(4, 1);
      lcd.print(msg);
      lcd.setCursor(13, 1);
      lcd.print(fee);
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print(cardId);
      lcd.setCursor(11, 0);
      lcd.print("M:2");
      lcd.setCursor(0, 1);
      lcd.print("Msg:     Add:   ");
      lcd.setCursor(4, 1);
      lcd.print(msg);
      lcd.setCursor(13, 1);
      lcd.print(add);
      break;
  }
}

void listenerButton() {
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
    clearLCDDefaultValue();
    tone(BUZEER_PIN, 2000, 100);
    mode++;
  }
  delay(300);
}

void clearLCDDefaultValue() {
  cardId = "          "; // 卡片 id
  balance = 0;
  msg = "";  
}
void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
  timer4.update();
}
