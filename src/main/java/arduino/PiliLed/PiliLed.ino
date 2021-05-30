const int P1_PIN = 13;
const int P2_PIN = 12;
const int P3_PIN = 11;
const int P4_PIN = 10;
const int P5_PIN = 9;
int curSpeed = 500;

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  pinMode(P1_PIN, OUTPUT);
  pinMode(P2_PIN, OUTPUT);
  pinMode(P3_PIN, OUTPUT);
  pinMode(P4_PIN, OUTPUT);
  pinMode(P5_PIN, OUTPUT);
}

void loop() {
  //Homework
  /*
    1:onAll(), 2:offAll(), 3:blinkLed(), 4:piliLed(), 5:antiPiliLed(), 6:randomLed()
    7:加速, 8:減速
  */
  //onAll()
  //offAll()
  //blinkLed();
  //piliLed();
  //antiPiliLed();
  //randomLed();
}

// 隨機燈
void randomLed() {
  int pin = random(9, 14);
  Serial.print("random = ");
  Serial.println(pin);
  offAll();
  digitalWrite(pin, HIGH);
  delay(curSpeed);
}

// 反霹靂燈
void antiPiliLed() {
  // 由 9 .. 13 由 下 往 上
  for (int pin = P5_PIN ; pin < P5_PIN + 5 ; pin++) {
    onAll();
    digitalWrite(pin, LOW);
    delay(curSpeed);
  }
  // 由 13 .. 9 由 上 往 下
  for (int pin = P5_PIN + 3 ; pin > P5_PIN ; pin--) {
    onAll();
    digitalWrite(pin, LOW);
    delay(curSpeed);
  }
}

// 霹靂燈
void piliLed() {
  // 由 9 .. 13 由 下 往 上
  for (int pin = P5_PIN ; pin < P5_PIN + 5 ; pin++) {
    offAll();
    digitalWrite(pin, HIGH);
    delay(curSpeed);
  }
  // 由 13 .. 9 由 上 往 下
  for (int pin = P5_PIN + 3 ; pin > P5_PIN ; pin--) {
    offAll();
    digitalWrite(pin, HIGH);
    delay(curSpeed);
  }
}

void offAll() {
  digitalWrite(P1_PIN, LOW);
  digitalWrite(P2_PIN, LOW);
  digitalWrite(P3_PIN, LOW);
  digitalWrite(P4_PIN, LOW);
  digitalWrite(P5_PIN, LOW);
}

void onAll() {
  digitalWrite(P1_PIN, HIGH);
  digitalWrite(P2_PIN, HIGH);
  digitalWrite(P3_PIN, HIGH);
  digitalWrite(P4_PIN, HIGH);
  digitalWrite(P5_PIN, HIGH);
}

// 閃爍
void blinkLed() {
  onAll();
  delay(curSpeed);
  offAll();
  delay(curSpeed);
}
