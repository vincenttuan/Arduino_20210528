/*
   將CDS與DHT11(溫濕度)都可以顯示在Tube上
   可以利用 Timer 來實現
   閃爍顯示
   0321 H50.0 26.2C
*/
#include <Timer.h>
#include <DHT.h>
#include <TridentTD_7Segs74HC595.h>

#define DHT_TYPE DHT11
#define SCLK 4
#define RCLK 5
#define DIO  6

const int DHT_PIN   = 2; // 使用數位腳 2
const int DHT_DELTA = -5;
const int CDS_PIN   = A3;
const int LED_PIN   = 7;
const int BUTTIN_PIN = 3;
const int BUZEER_PIN = 8;

DHT dht(DHT_PIN, DHT_TYPE); // 初始化 dht
TridentTD_7Segs74HC595  tube( SCLK, RCLK, DIO); // 初始化 tube
Timer timer1, timer2, timer3, timer4, timer5, timer6; // 宣告 Timer 物件

int   cdsValueThreshold = 500; // 門檻值(小於：燈亮，大於：燈滅)
int   cdsValue          = 0;   // CDS資料
float humiValue         = 0.0; // 濕度資料
float tempValue         = 0.0; // 溫度資料
int tubeState = 0;
int perButtonState = 0;

void setup() {
  delay(100);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTIN_PIN, INPUT);
  pinMode(BUZEER_PIN, OUTPUT);
  
  dht.begin(); // 啟動 dht
  tube.init();  // 啟動 tube

  // 配置排程
  timer1.every(100,  listenerButton); // 每100ms監聽Button狀態乙次
  timer2.every(1000, setCDS);         // 每1000ms取得CDS資料乙次
  timer3.every(1000, checkLED);       // 每1000ms檢查是否要開燈
  timer4.every(500,  updateCDSValueThreshold); // 修改 cdsValueThreshold
  timer5.every(1000, setDHT);         // 每1000ms取得最新溫濕度乙次
  timer6.every(500,  playTube);       // 每500ms執行Tube顯示乙次

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
      tubeState++;
      Serial.print("tubeState = ");
      Serial.println(tubeState);
    }
    perButtonState = curButtonState; // 狀態交換
  }

}
// 取得最新 CDS 資料
void setCDS() {
  cdsValue = analogRead(CDS_PIN); // 0~1023
  Serial.print("CDS: ");
  Serial.println(cdsValue);
}

// 檢查是否要開燈
void checkLED() {
  if (cdsValue < cdsValueThreshold) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

// 修改 cdsValueThreshold
void updateCDSValueThreshold() {
  if (Serial.available() > 0) {
    String data = Serial.readString();
    data.trim();
    cdsValueThreshold = data.toInt();
    Serial.println(cdsValueThreshold);
  }
}

// 取得最新溫濕度
void setDHT() {
  humiValue = dht.readHumidity(); // 濕度
  tempValue = dht.readTemperature(false); // 溫度 true: 華氏, false(預設): 攝氏
  if (isnan(humiValue) || isnan(tempValue)) { // nan -> not a number
    Serial.println("No dht data !!");
    delay(1000);
    return;
  }
  // 溫度 delta 調整
  tempValue = tempValue + DHT_DELTA;
  Serial.print("H:");
  Serial.print(humiValue, 1);
  Serial.print("% T:");
  Serial.print(tempValue, 2);
  Serial.println("*C");
}

// 執行Tube
void playTube() {
  //tube.setText(String(cdsValue));
  switch (tubeState % 3) {
    case 1:
      if (isnan(cdsValue)) break;
      tube.setText(String(cdsValue));
      break;
    case 2:
      if (isnan(humiValue)) break;
      tube.setText(String(humiValue));
      break;
    default:
      if (isnan(tempValue)) break;
      tube.setText(String(tempValue));
      break;
  }

}

void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
  timer4.update();
  timer5.update();
  timer6.update();
}
