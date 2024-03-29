const int BUTTIN_ON_PIN = 7; // 開燈按鈕的接腳(PIN)
const int BUTTIN_OFF_PIN = 12; // 關燈按鈕的接腳(PIN)
const int LED_PIN = 9; // LED 燈的接腳

int ledState = 0; // LED開/關狀態值, Ex: ledState = 1 (開燈), 0 (關燈)

void setup() {
  Serial.begin(9600);
  pinMode(BUTTIN_ON_PIN, INPUT); // 設定按鈕的接腳為輸入，因為我們要讀取他的狀態值
  pinMode(BUTTIN_OFF_PIN, INPUT); // 設定按鈕的接腳為輸入，因為我們要讀取他的狀態值
  pinMode(LED_PIN, OUTPUT); // 設定LED的接腳為輸出
}

void loop() {
  // 判斷是否 BUTTIN_ON_PIN 被按下(開燈)
  if(digitalRead(BUTTIN_ON_PIN) == LOW) { // LOW = 0, HIGH = 1
      ledState = 1; // ledState = 1 (開燈)
      Serial.println("ON");
  }

  // 判斷是否 BUTTIN_OFF_PIN 被按下(關燈)
  if(digitalRead(BUTTIN_OFF_PIN) == LOW) { // LOW = 0, HIGH = 1
      ledState = 0; // ledState = 0 (關燈)
      Serial.println("OFF");
  }

  // 判斷 ledState 的狀態 
  if(ledState == 1) { // ledState = 1 (開燈), 0 (關燈)
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(100);
}
