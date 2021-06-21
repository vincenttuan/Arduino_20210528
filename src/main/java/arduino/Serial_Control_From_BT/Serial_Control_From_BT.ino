/* 
  藍牙收送程式 HC-05
  接線方式:
  RXD: D6
  TXD: D5
  GND: GND
  VCC: +5v
  ----------------------------------
  R:1 -> D2
  G:2 -> D3
  S:4(Open),
    8(Close) -> D4 (紅+5V,菊D4,咖GND)
  Python 傳送指定數值可以控制設備
  傳送過來的值要顯示在 LCD 上
*/
#include <SoftwareSerial.h>
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
SoftwareSerial BTSerial(5, 6); // (RX, TX)
Servo myservo; // 建立 Servo 物件
int openDegree  = 15;  // 開門角度
int closeDegree = 105; // 關門角度
int curDegree   = closeDegree;

Timer timer1, timer2, timer3, timer4;
int control = 0;
String inString = "";

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600); // 5, 6 接腳 (9600 或 38400)
  pinMode(RED_PIN, OUTPUT);
  pinMode(GEEEN_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:");
  
  myservo.attach(SERVO_PIN); // 設定 Servo 訊號腳位
  myservo.write(curDegree); // 0~180
  
  timer1.every(100,  listenerSerial);
  timer2.every(100,  listenerBTSerial);
  timer3.every(100,  playLED);
  timer4.every(100,  playServo);
}

void loop() {
  timer1.update();
  timer2.update();
  timer3.update();
  timer4.update();
}

void playLED() {
  digitalWrite(RED_PIN,   control & 1);
  digitalWrite(GEEEN_PIN, control & 2);
}

void playServo() {
  // 開門
  if((control & 4) > 0 && curDegree == closeDegree) {
    for (int i = closeDegree; i >= openDegree; i -= 5) {
      myservo.write(i);
      curDegree = i;
      delay(50);
    }
  }
  
  // 關門
  if((control & 8) > 0 && curDegree == openDegree) {
    for (int i = openDegree; i <= closeDegree; i += 5) {
      myservo.write(i);
      curDegree = i;
      delay(50);
    }
  }
  
}

void listenerBTSerial() {
  while(BTSerial.available() > 0) {
    int inChar = BTSerial.read();
    if(isDigit(inChar)) {
        inString += (char)inChar;
    } else if((char)inChar != '#') {
        switch(inChar) {
          case 97: // a
            control++;
            break;
          case 115: // s
            control--;
            break;
          case 101: // e
            control = 0;
            break;
          case 112: // p
            control = 7; 
            break;
        }
        lcd.setCursor(2, 0);
        lcd.print("                ");
        lcd.setCursor(2, 0);
        lcd.print(control);
        inString = "";
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
