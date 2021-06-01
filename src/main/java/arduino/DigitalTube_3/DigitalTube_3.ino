#include <DigitalTube.h>

DigitalTube dis(4, 5, 6);

void setup() {
  Serial.begin(9600);
  dis.begin();
  Serial.println("Ready");
}

void loop() {
  //float f = 24.37;
  // 在 DigitalTube 能顯示 2437
  //dis.print(f*100);
  float temp = 23.4;
  //在 DigitalTube 能顯示 243C
  // ASCII 48->'0', 49->'1', 65->'A', 66->'B', 97->'a', 98->'b'
  int tempInt = (int)(temp*10); // 234
  //dis.show(tempInt/100, tempInt/10%10, tempInt%10, 'C');
  //在 DigitalTube 能顯示 24.3C
  //dis.show(1+10, 2+10, 3+10, 4+10);
  dis.show(tempInt/100, tempInt/10%10 + 10, tempInt%10, 'C');
  
  delay(2000);
}
