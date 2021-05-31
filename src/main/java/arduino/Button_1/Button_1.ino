const int BUTTIN_PIN = 7; // 按鈕的接腳(PIN)
const int LED_PIN = 9; // LED 燈的接腳

int buttonState = 0; // 按鈕的狀態值

void setup() {
  Serial.begin(9600);
  pinMode(BUTTIN_PIN, INPUT); // 設定按鈕的接腳為輸入，因為我們要讀取他的狀態值
  pinMode(LED_PIN, OUTPUT); // 設定LED的接腳為輸出
}

void loop() {
  buttonState = digitalRead(BUTTIN_PIN); // 讀取按鈕的狀態值
  Serial.println(buttonState);
  if(buttonState == LOW) { // LOW = 0, HIGH = 1
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
