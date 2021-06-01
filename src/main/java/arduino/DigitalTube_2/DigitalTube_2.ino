#include <DigitalTube.h>

DigitalTube dis(4, 5, 6);

void setup() {
  Serial.begin(9600);
  dis.begin();
  Serial.println("Ready");
}

void loop() {
  // Show Text
  // Hello 跑馬燈
  dis.show(' ', ' ', ' ', ' ');
  delay(500);
  dis.show(' ', ' ', ' ', 'H');
  delay(500);
  dis.show(' ', ' ', 'H', 'e');
  delay(500);
  dis.show(' ', 'H', 'e', 'l');
  delay(500);
  dis.show('H', 'e', 'l', 'l');
  delay(500);
  dis.show('l', 'l', 'o', ' ');
  delay(500);
  dis.show('l', 'o', ' ', ' ');
  delay(500);
  dis.show('o', ' ', ' ', ' ');
  delay(500);
  dis.show(' ', ' ', ' ', ' ');
  delay(500);

  for(int i=9;i>0;i--) {
    dis.print(i);  
    delay(500);
  }

  dis.show('S', 'T', 'O', 'P');
  delay(300);
  dis.show(' ', ' ', ' ', ' ');
  delay(300);
  dis.show('S', 'T', 'O', 'P');
  delay(300);
  dis.show(' ', ' ', ' ', ' ');
  delay(300);
  dis.show('S', 'T', 'O', 'P');
  delay(300);
  dis.show(' ', ' ', ' ', ' ');
  delay(2000);
  
}
