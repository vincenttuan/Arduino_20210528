                 // SGR    R   G   S     GR  S R  SG   SGR
                 // 000  001  010  100  011  101  110  111
int control = 0; //   0    1    2    4    3    5    6    7
void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) {
    int n = Serial.read() - 48;
    if(n >= 0) {
      Serial.println(n);
      // 011 = 3
      // 011 & 001 = 001
      // 110 = 6
      // 110 & 001 = 000
      Serial.println(n & 1);
      Serial.println(n & 2);
      Serial.println(n & 4);
      Serial.println((n & 1) > 0 ? "R on" : "R off");
      Serial.println((n & 2) > 0 ? "G on" : "G off");
      Serial.println((n & 4) > 0 ? "S on" : "S off");
    }    
  }
  delay(100);
}
