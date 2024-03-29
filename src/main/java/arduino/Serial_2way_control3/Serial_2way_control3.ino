// 若 arduino 收到 '+' 每次 -50
// 若 arduino 收到 '-' 每次 +50
// 若 arduino 收到 'r' 字元表示還原 200ms 的速度
int i = 1;
int initSpeed = 200;
int curSpeed = initSpeed;
int delta = 50;
boolean play = false;
void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  i++;
  if(Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case '1':
        play = true;
        break;  
      case '2':
        play = false;
        break;  
      case '+':
        curSpeed -= delta;
        if(curSpeed < 0) {
            curSpeed = 0;
        }
        break;  
      case '-':
        curSpeed += delta;
        if(curSpeed > 10000) {
            curSpeed = 10000;
        }
        break;
      case 'r':
        curSpeed = initSpeed;
        break;    
    }  
  }
  if(play) { // play == true
    Serial.println(i);
  }
  delay(curSpeed); // ms
}
