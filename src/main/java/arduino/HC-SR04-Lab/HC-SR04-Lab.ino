/*
  偵測距離在10cm~5cm之間亮紅燈發出長音bee~~~bee~~ ...
  偵測距離在 < 5cm 亮黃燈連續短音bee~bee~ ...
  偵測距離 > 10cm 不亮燈不發音
  距離請顯示在 DigitalTube
*/
#include <Timer.h>
#include <Ultrasonic.h>
#include <TridentTD_7Segs74HC595.h>
#define SCLK 4
#define RCLK 5
#define DIO  6
#define LED_YELLOW_PIN 9
#define LED_RED_PIN 10
#define BUZEER_PIN  8
#define TRIG_PIN 12
#define ECHO_PIN 11

TridentTD_7Segs74HC595  tube(SCLK, RCLK, DIO); // 初始化 tube
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN); // 初始化 HC-SR04
Timer timer1, timer2, timer3; // 建立 Timer

int distance;

void setup() {
  delay(100);
  Serial.begin(9600);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(BUZEER_PIN, OUTPUT);
  tube.init();  // 啟動 tube

  // 配置排程
  timer1.every(200, detect);
  timer2.every(200, checkDetect);
  timer2.every(200, playTube);
}

void checkDetect() {
  if (distance < 1) delay(100);
  if (distance < 5) {
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, LOW);
    tone(BUZEER_PIN, 2000, 100);
    delay(100);
  } else if (distance < 10) {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    tone(BUZEER_PIN, 2000, 100);
    delay(400);
  } else {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    delay(200);
  }
}

void detect() {
  
  distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(200); //每次間格0.2秒
}

void playTube() {
  if (isnan(distance)) return;
  tube.setText(String(distance));
}

void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
}
