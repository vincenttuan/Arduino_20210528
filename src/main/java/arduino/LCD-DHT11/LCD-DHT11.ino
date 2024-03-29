/*
將 DHT11 的溫濕度顯示在 LCD 螢幕上
按鈕可以控制LCD背光開啟
寫完請將 LCD 的畫面上傳
*/
#include <Timer.h>
#include <DHT.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define DHT_TYPE   DHT11
#define DHT_PIN    2 // 使用數位腳 2
#define DHT_DELTA  5
#define BUTTIN_PIN 3
#define BUZEER_PIN 8
#define I2C_ADDR   0x27

DHT   dht(DHT_PIN, DHT_TYPE); // 初始化 dht
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Timer timer1, timer2, timer3; // 宣告 Timer 物件

float humiValue      = 0.0; // 濕度資料
float tempValue      = 0.0; // 溫度資料
int   buttonCount    = 0;   // 計數 button 按下的次數
int   perButtonState = 0;   // 前一次 button 的狀態

void setup() {
  delay(10);
  Serial.begin(9600);
  pinMode(BUTTIN_PIN, INPUT);
  pinMode(BUZEER_PIN, OUTPUT);
  
  dht.begin(); // 啟動 dht

  // 初始 LCD
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  /*
  LCD 佈局
  ---------------
  H:00.0 %
  T:00.00 *C
  ---------------
  */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("H:00.0 %");
  lcd.setCursor(0, 1);
  lcd.print("T:00.00 *C");
  
  // 配置排程
  timer1.every(100,  listenerButton); // 每100ms監聽Button狀態乙次
  timer2.every(1000, setDHT);         // 每1000ms取得最新溫濕度乙次
  timer3.every(500,  playLCD);        // 每500ms執行LCD顯示乙次

  Serial.println("Ready");
  
}

// 監聽 button
void listenerButton() {
  int curButtonState = digitalRead(BUTTIN_PIN); // 目前 Button 的狀態
  if(curButtonState == LOW) {
    tone(BUZEER_PIN, 2000, 100);
    delay(300);
  }
  if (curButtonState != perButtonState) {
    if (curButtonState == LOW) { // LOW = 0, HIGH = 1
      buttonCount++; // 給 LCD 背光作為 on/off 的參考
      if(buttonCount % 2 == 1) {
        lcd.setBacklight(LOW);  
      } else {
        lcd.setBacklight(HIGH);
      }
    }
    perButtonState = curButtonState; // 狀態交換
  }

}

// 取得最新溫濕度
void setDHT() {
  humiValue = dht.readHumidity(); // 濕度
  tempValue = dht.readTemperature(false); // 溫度 true: 華氏, false(預設): 攝氏
  if (isnan(humiValue) || isnan(tempValue)) { // nan -> not a number
    delay(1000);
    return;
  }
  // 溫度 delta 調整
  tempValue = tempValue + DHT_DELTA;
  
}

// 執行LCD
void playLCD() {
  Serial.print("H:");
  Serial.print(humiValue, 1);
  Serial.println("%");
  Serial.print("T:");
  Serial.print(tempValue, 2);
  Serial.println("*C");
  /*
  LCD 佈局
  ---------------
  H:00.0 %
  T:00.00 *C
  ---------------
  */
  lcd.setCursor(2, 0);
  lcd.print("     ");
  lcd.setCursor(2, 0);
  lcd.print(humiValue, 1);
  lcd.setCursor(2, 1);
  lcd.print("     ");
  lcd.setCursor(2, 1);
  lcd.print(tempValue, 2);
}

void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
}
