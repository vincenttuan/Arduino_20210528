/*
  R:1 -> D2
  G:2 -> D3
  S:4(Open),
    8(Close) -> D4 (紅+5V,菊D4,咖GND)
  Python 傳送指定數值可以控制設備
  傳送過來的值要顯示在 LCD 上
*/
#include <Servo.h>
#include <Timer.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define RED_PIN 2
#define GEEEN_PIN 3
#define SERVO_PIN 4
#define I2C_ADDR 0x27

LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Servo myservo; // 建立 Servo 物件
int initDegree = 15;
int maxDegree = 105;
int curDegree = initDegree;

Timer timer1, timer2, timer3;
int control = 0;
String inString = "";

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GEEEN_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:");
  
  myservo.attach(SERVO_PIN); // 設定 Servo 訊號腳位
  myservo.write(initDegree); // 0~180
  
  timer1.every(100,  listenerSerial);
  timer2.every(100,  playLED);
  //timer3.every(100,  playServo);
}

void loop() {
  timer1.update();
  timer2.update();
  //timer3.update();
}

void playLED() {
  digitalWrite(RED_PIN,   control & 1);
  digitalWrite(GEEEN_PIN, control & 2);
}

void playServo() {
  for (int i = initDegree; i <= maxDegree; i += 5) {
    myservo.write(i);
    Serial.println(i);
    delay(50);
  }
  delay(3000);
  for (int i = maxDegree; i >= initDegree; i -= 5) {
    myservo.write(i);
    Serial.println(i);
    delay(50);
  }
}

void listenerSerial() {
  while(Serial.available() > 0) {
    int inChar = Serial.read();
    if(isDigit(inChar)) {
        inString += (char)inChar;
    }
    if(inChar == '\n' || inChar == '#') { // "#" 或 "\n" 表示結束字元
        lcd.setCursor(2, 0);
        lcd.print("                ");
        lcd.setCursor(2, 0);
        control = inString.toInt();
        lcd.print(control);
        inString = "";
    }
  }
  delay(50);
}
