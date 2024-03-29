/*
  輸入 1: 啟動 Blink, 輸入 2: 關閉 Blink(關燈), 輸入 3: 加速閃爍, 輸入 4: 減速閃爍, 輸入: 5 恆亮
*/
int led_pin   = 13;
int play      = false; // 啟動
int minSpeed  = 100;
int maxSpeed  = 10000;
int initSpeed = 1000;
int curSpeed  = initSpeed;
int delta     = 100;   // 調整 delay(ms) 的參數
int alwaysOn  = false; // 恆亮
void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  // 設定數位13腳號 = OUTPUT
  pinMode(led_pin, OUTPUT);
  
}

void loop() {

  // 讀取資料來源
  readData();
  // 執行 LED 效果
  playLED();
}

// 讀取資料來源
void readData() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch (c) {
      case '1':
        play = true;
        alwaysOn = false;
        break;
      case '2':
        play = false;
        break;
      case '3':
        curSpeed -= delta;
        if (curSpeed < minSpeed) curSpeed = minSpeed;
        break;
      case '4':
        curSpeed += 100;
        if (curSpeed >= maxSpeed) curSpeed = maxSpeed;
        break;
      case '5':
        alwaysOn = true;
        break;
    }
  }
}

// 執行 LED 效果
void playLED() {
  if(play) {
    if (alwaysOn) { // 恆亮
      // 數位訊號寫入
      digitalWrite(led_pin, HIGH);
    } else {  // 閃爍 Blink
      digitalWrite(led_pin, HIGH);
      delay(curSpeed);
      digitalWrite(led_pin, LOW);
      delay(curSpeed);
    }
  } else {
    digitalWrite(led_pin, LOW);  
  }

}
