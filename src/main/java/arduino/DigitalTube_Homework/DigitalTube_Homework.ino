// 透過 PC 輸入 STOP  顯示 STOP
// 透過 PC 輸入 1234  顯示 1234
// 透過 PC 輸入 1.234 顯示 1.234
// 透過 PC 輸入 12.34 顯示 12.34
// 透過 PC 輸入 123.4 顯示 123.4
// 接線方式
/*
  GND: GND
  dio: 6
  rc: 5
  sc: 4
  VCC: 5V
*/
#include <DigitalTube.h>

DigitalTube dis(4, 5, 6); // sclk, rclk, dio

void setup() {
  delay(10);
  Serial.begin(9600);
  dis.begin();
  Serial.println("Ready");
}

void loop() {
  if(Serial.available() > 0) {
    String data = Serial.readString();
//    Serial.println(data.charAt(0));
//    Serial.println(data.charAt(1));
//    Serial.println(data.charAt(2));
//    Serial.println(data.charAt(3));
    Serial.print(data);
    Serial.println(data.indexOf('.'));
    int dotIdx = data.indexOf('.');
    if(dotIdx == -1) { // 沒有 '.'
      dis.show(data.charAt(0), data.charAt(1), data.charAt(2), data.charAt(3));
    } else {
      switch(dotIdx) {
        case 0: // 若「.」位置 = 0 例如: ".123" 顯示: 0.123
          dis.show(10+0, data.charAt(1), data.charAt(2), data.charAt(3));
          break;
        case 1: // 若「.」位置 = 1 例如: "2.345" 顯示: 2.345
          // ASCII 48 = 0, 49 = 1, 50 = 2, 51 = 3 ...
          dis.show(10+((int)data.charAt(0))-48, data.charAt(2), data.charAt(3), data.charAt(4));
          break; 
        case 2: // 若「.」位置 = 2 例如: "23.45" 顯示: 23.45
          dis.show(data.charAt(0), 10+((int)data.charAt(1))-48, data.charAt(3), data.charAt(4));
          break;
        case 3: // 若「.」位置 = 3 例如: "234.5" 顯示: 234.5
          dis.show(data.charAt(0), data.charAt(1), 10+((int)data.charAt(2))-48, data.charAt(4));
          break;
        default:
          dis.show('E', 'R', 'R', ' ');
      }
    }
  }
  delay(100);
}
