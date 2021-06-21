                 // SGR    R   G   S     GR  S R  SG   SGR
                 // 000  001  010  100  011  101  110  111
int control = 7; //   0    1    2    4    3    5    6    7
void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) {
    int n = Serial.read() - 48;
    if(n >= 0) {
      control = control ^ n;
      Serial.println(control);
      Serial.println((control & 1) > 0 ? "R on" : "R off");
      Serial.println((control & 2) > 0 ? "G on" : "G off");
      Serial.println((control & 4) > 0 ? "S on" : "S off");
      
    }    
  }
  delay(100);
}
