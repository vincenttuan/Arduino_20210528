const int TRIG_PIN = 12;
const int ECHO_PIN = 11;

void setup() {
  delay(10);
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("Ready");
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH); // 發送: 給 HIGH 持續 10ms
  delayMicroseconds(10); // 持續 10ms
  digitalWrite(TRIG_PIN, LOW);

  int duration = pulseIn(ECHO_PIN, HIGH); // pulseIn ( ) ：讀取一個針腳的脈衝時間
  // 在攝氏0度下音速每一公分需要29.1ms
  int cm = (duration/2) / 29.1;
  
  Serial.print("duration: ");
  Serial.print(duration);
  Serial.print(", cm: ");
  Serial.println(cm);
  delay(200);
}
