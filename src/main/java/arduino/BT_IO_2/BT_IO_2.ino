/*
  藍牙收送程式 HC-05
  接線方式:
  RXD: D1 (TX -> 1)
  TXD: D0 (RX <- 0)
  GND: GND
  VCC: +5v
*/
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN 7

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(9600);   // 0, 1 接腳
  pinMode(BUTTON_PIN, INPUT);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RX:");
  lcd.setCursor(0, 1);
  lcd.print("TX:");
}

void loop() {
  // 檢查是否有資料透過(RX:0,TX:1)傳進來
  if (Serial.available()) {
    String data = Serial.readString(); // 讀取(RX:0,TX:1)傳來的資料
    lcd.setCursor(3, 0);
    lcd.print("             ");
    lcd.setCursor(3, 0);
    lcd.print(data);
  }
  // 透過(RX:0,TX:1)將資料送出
  if (digitalRead(BUTTON_PIN) == LOW) {
    int n = random(0, 100);
    Serial.println(n);
    lcd.setCursor(3, 1);
    lcd.print("             ");
    lcd.setCursor(3, 1);
    lcd.print(n);
    delay(300);
  }
  
  delay(200);

}
