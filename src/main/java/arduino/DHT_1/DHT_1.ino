#include "DHT.h"
#define DHT_TYPE DHT11
const int DHT_PIN = 2; // 使用數位腳 2
const int DHT_DELTA = -5;
DHT dht(DHT_PIN, DHT_TYPE); // 初始化 dht

void setup() {
  delay(100);
  Serial.begin(9600);
  dht.begin(); // 啟動 dht
  Serial.println("Ready");
  
}

void loop() {
  float h = dht.readHumidity(); // 濕度
  float t = dht.readTemperature(false); // 溫度 true: 華氏, false(預設): 攝氏
  if(isnan(h) || isnan(t)) { // nan -> not a number
    Serial.println("No dht data !!");
    delay(1000);
    return;
  }
  // 溫度 delta 調整
  t = t + DHT_DELTA;
  Serial.print("H:");
  Serial.print(h, 1);
  Serial.print("% T:");
  Serial.print(t, 2);
  Serial.println("*C");
  delay(1000);
}
