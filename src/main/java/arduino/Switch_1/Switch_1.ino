/*
磁簧開關:
任意一條接 GND 另一條 D13
*/
#define SWITCH_PIN 13

int switchValue;

void setup() {
  delay(10);
  Serial.begin(9600);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  Serial.println("Ready");
}

void loop() {
  switchValue = digitalRead(SWITCH_PIN);
  Serial.print("switchValue= ");
  Serial.println(switchValue);
  delay(1000);
}
