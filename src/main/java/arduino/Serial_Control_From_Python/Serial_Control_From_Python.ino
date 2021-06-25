/*
    LCD 螢幕資料
   ---------------------------------
   R:1 28.30,89.00 (接收自Python)
   756,20.10,57.00 (回應給Python)
   ---------------------------------
   接收指令
   LED RED  :1 -> D2
   LED GREEN:2 -> D3
   Servo    :4(Open),
             8(Close) -> D4 (紅+5V,菊D4,咖GND)

   Python 傳送指定數值可以控制設備
   傳送過來的值要顯示在 LCD 上
   Buzeer => GND -> GND
             I/O -> D6
             VCC -> +5V
   BUTTON => D7
   ----------------------------------
   傳送資料
   CDS => GND  -> GND
          VCC  -> +5V
          SIG  -> A3
   DHT => GND  -> GND
          DATA -> D5
          VCC  -> +5V

*/
#include <Servo.h>
#include <Timer.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define RED_PIN 2
#define GEEEN_PIN 3
#define SERVO_PIN 4
#define I2C_ADDR 0x27
#define CDS_PIN A3
#define DHT_TYPE DHT11
#define DHT_PIN 5
#define BUZEER_PIN 6
#define BUTTON_PIN 7

LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Timer timer1, timer2, timer3, timer4, timer5, timer6;
Servo myservo; // 建立 Servo 物件
DHT dht(DHT_PIN, DHT_TYPE); // 初始化 dht

int DHT_DELTA = -5;
int openDegree  = 15;  // 開門角度
int closeDegree = 105; // 關門角度
int curDegree   = openDegree;
int cdsValue = 0;
float humi = 0.0;
float temp = 0.0;

int control = 0;
int minControl = 0;
int maxControl = 255;
String inString = "";

boolean isBuzeer = false;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GEEEN_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(BUZEER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:");

  myservo.attach(SERVO_PIN); // 設定 Servo 訊號腳位
  myservo.write(curDegree); // 0~180

  dht.begin(); // 啟動 dht

  timer1.every(100,  listenerSerial);
  timer2.every(100,  playLED);
  timer3.every(100,  playServo);
  timer4.every(100,  playBuzeerAndButton);
  timer5.every(100,  playDHT11AndCDS);
  timer6.every(100,  responseData);
}

void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
  timer4.update();
  timer5.update();
  timer6.update();
}

void responseData() {
  Serial.print(cdsValue);
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(humi);
  Serial.print(",");
  Serial.print(control);
  Serial.print(",");
  Serial.print(curDegree);
  Serial.println();
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(cdsValue);
  lcd.print(", ");
  lcd.print(temp);
  lcd.print(",");
  lcd.print(humi);
}

void playDHT11AndCDS() {
  cdsValue = analogRead(CDS_PIN);
  float h = dht.readHumidity(); // 濕度
  float t = dht.readTemperature(false); // 溫度 true: 華氏, false(預設): 攝氏
  if (isnan(h) || isnan(t)) { // nan -> not a number
    delay(500);
    return;
  }
  humi = h;
  // 溫度 delta 調整
  temp = t + DHT_DELTA;
}

void playBuzeerAndButton() {
  if (control == 16) { // 開警報
    isBuzeer = true;
  }
  if (isBuzeer) {
    tone(BUZEER_PIN, 2500, 500);
    delay(200);
  }
  if (digitalRead(BUTTON_PIN) == LOW) { // LOW = 0, HIGH = 1
    control = 32; // 關警報
    isBuzeer = false;
  }
}

void playLED() {
  digitalWrite(RED_PIN,   control & 1);
  digitalWrite(GEEEN_PIN, control & 2);
}

void playServo() {
  // 開門
  if ((control & 4) > 0 && curDegree == closeDegree) {
    for (int i = closeDegree; i >= openDegree; i -= 5) {
      myservo.write(i);
      curDegree = i;
      delay(50);
    }
  }

  // 關門
  if ((control & 8) > 0 && curDegree == openDegree) {
    for (int i = openDegree; i <= closeDegree; i += 5) {
      myservo.write(i);
      curDegree = i;
      delay(50);
    }
  }

}

boolean ow = false;
String owString = "";
//ex: A23.56,78.12#
void listenerSerial() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (inChar == 65) { // A
      owString = "";
      ow = true;
    } else if (isDigit(inChar) && ow == false) {
      inString += (char)inChar;
    }

    if (ow == true) {
      owString += (char)inChar;
    }

    if (inChar == '\n' || inChar == '#' && ow == true) {
      lcd.setCursor(5, 0);
      lcd.print("            ");
      lcd.setCursor(5, 0);
      lcd.print(owString.substring(1, owString.length() - 1)); // ex: A23.56,78.12# -> 23.56,78.12
      owString = "";
      ow = false;
    } else if (inChar == '\n' || inChar == '#' && ow == false) { // "#" 或 "\n" 表示結束字元
      int tmpControl = inString.toInt();
      if (tmpControl >= minControl && tmpControl <= maxControl) {
        control = tmpControl;
        lcd.setCursor(2, 0);
        lcd.print("  ");
        lcd.setCursor(2, 0);
        lcd.print(control);
      }
      inString = "";
    }
  }
  delay(50);
}
