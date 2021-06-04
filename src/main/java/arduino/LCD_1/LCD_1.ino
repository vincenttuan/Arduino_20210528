/*
 * LCD i2c 接線
 * GND - GND
 * VCC - +5V
 * SDA - A4
 * SCL - A5
 * 
*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27 // 0x27或0x3F 定義 i2c 的 Address 位置 (PCF8574T) 

/*
   En, Rw, Rs, d4, d5, d6, d7, BackLight_Pin
   2,  1,  0,  4,  5,  6,  7,  3
*/
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  delay(10);
  Serial.begin(9600);
  lcd.begin(16, 2);
//  lcd.setBacklightPin(3, POSITIVE); // 設定背光(POSITIVE 積極的)
//  lcd.backlight(); // 開啟背光
  lcd.setBacklight(HIGH); // HIGH/LOW 控制 LCD 的背光
  // 清空 LCD
  lcd.clear();
  // 設定 cursor
  lcd.setCursor(5, 0); // (欄0-15, 列0-1)
  // 印字
  lcd.print("Hello!");
  // 設定 cursor
  lcd.setCursor(0, 1); // (欄0-15, 列0-1)
  // 印字
  lcd.print("*LCD@#$%^&.,/()-");
}

void loop() {
  
}
