/*
  PWM LED RGB + BUTTON Lab
  1.每短按一次＝輪流出現 RED, GREEN, BLUE
  2.若長按一次=關燈
*/

const int LED_BLUE_PIN     = 9;   // LED BLUE   燈的接腳( 9 = PIN(PWM))
const int LED_RED_PIN      = 10;  // LED RED    燈的接腳(10 = PIN(PWM))
const int LED_GEEEN_PIN    = 11;  // LED GEEEN  燈的接腳(11 = PIN(PWM))
const int BUTTON_PIN       = 7;   // 開/關燈按鈕的接腳(PIN)
const int DELAY            = 100; // 暫停時間 ms
const int LONG_CLICK_COUNT = 10;  // 長按次數

int rgbColor = 1;    // RGB 顏色
int buttonCount = 0; // 累積按(下)次數

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GEEEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // 預設全滅
  analogWrite(LED_RED_PIN, 255);
  analogWrite(LED_GEEEN_PIN, 255);
  analogWrite(LED_BLUE_PIN, 255);
}

void loop() {

  if (digitalRead(BUTTON_PIN) == LOW) {
    if(buttonCount == 0) {
      analogWrite(LED_RED_PIN,   rgbColor%3==1?255:0 );
      analogWrite(LED_GEEEN_PIN, rgbColor%3==2?255:0 );
      analogWrite(LED_BLUE_PIN,  rgbColor%3==0?255:0 );
      rgbColor++; // rgbColor 累加
    }
    buttonCount++; // 累積按(下)次數
  }

  // 判斷是否符合長按 ?
  if (buttonCount >= LONG_CLICK_COUNT) {
    analogWrite(LED_RED_PIN, 255);
    analogWrite(LED_GEEEN_PIN, 255);
    analogWrite(LED_BLUE_PIN, 255);
  }

  // 判斷在符合長按後放開BUTTON
  if(buttonCount >= LONG_CLICK_COUNT && digitalRead(BUTTON_PIN) == HIGH) {
      buttonCount = 0;
  }

  // 判斷在符合短按後放開BUTTON
  if(buttonCount < LONG_CLICK_COUNT && digitalRead(BUTTON_PIN) == HIGH) {
      buttonCount = 0;
  }

  Serial.print("buttonCount= ");
  Serial.println(buttonCount);

  delay(DELAY);
}
