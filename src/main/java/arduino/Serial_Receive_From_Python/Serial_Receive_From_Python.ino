/*
 * 1.請查看板子上的 RX 燈號
 * 2.請將收到的資料顯示在 LCD 螢幕上
 */
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27

LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
String inString = "";

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R:");
}

void loop() {
  while(Serial.available() > 0) {
    int inChar = Serial.read();
    if(isDigit(inChar)) {
        inString += (char)inChar;
    }
    if(inChar == '#') { // "#"表示結束字元
        lcd.setCursor(2, 0);
        lcd.print("   ");
        lcd.setCursor(2, 0);
        lcd.print(inString);
        inString = "";
    }
  }
  delay(50);

}
