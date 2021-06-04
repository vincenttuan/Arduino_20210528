#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27 // 0x27或0x3F 定義 i2c 的 Address 位置 (PCF8574T) 
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  delay(10);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH); // HIGH/LOW 控制 LCD 的背光
  // 清空 LCD
  lcd.clear();
  // 設定 cursor
  lcd.setCursor(0, 0); // (欄0-15, 列0-1)
  // 印字
  lcd.print("Lotto:");
}

void loop() {
  int n = random(1, 200);
  Serial.println(n);
  lcd.setCursor(6, 0);
  lcd.print("   ");
  lcd.setCursor(6, 0);
  lcd.print(n);
  delay(200);
}
