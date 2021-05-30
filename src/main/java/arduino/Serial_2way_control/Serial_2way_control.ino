// 若 arduino 收到 "on"  表示一直印出 i 的值
// 若 arduino 收到 "off" 表示停止印出 i 的值
int i = 1;
boolean play = false; // 控制是否要顯示 i 值的變數
void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  i++;
  if(Serial.available() > 0) {
    String str = Serial.readString(); // "on\n" 或 off\n
    Serial.print(str);
    if(str == "on\n") {
        play = true;
    } else if(str == "off\n") {
        play = false;
    }
  }
  if(play) { // play == true
    Serial.println(i);
  }
  delay(200);
}
