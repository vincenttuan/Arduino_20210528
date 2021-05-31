// LED RGB 燈
// 共陽接+5/共陰接GND
// 9: BLUE, 10: RED, 11: GREEN
const int LED_BLUE_PIN   = 9;    // LED BLUE   燈的接腳( 9 = PIN(PWM))
const int LED_RED_PIN    = 10;   // LED RED 燈的接腳(10 = PIN(PWM))
const int LED_GEEEN_PIN  = 11;   // LED GEEEN  燈的接腳(11 = PIN(PWM))
const int DELAY          = 500;  // 暫停時間 ms

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GEEEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
}

void loop() {
  // 取隨機值
  int r = random(0, 256);
  int g = random(0, 256);
  int b = random(0, 256);

  Serial.print("R:");
  Serial.print(r);
  Serial.print(" G:");
  Serial.print(g);
  Serial.print(" B:");
  Serial.println(b);
  
  analogWrite(LED_RED_PIN, r);
  analogWrite(LED_GEEEN_PIN, g);
  analogWrite(LED_BLUE_PIN, b);
  
  
  delay(DELAY);

}
