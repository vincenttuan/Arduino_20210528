#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 11); // TrigPin, EchoPin
int distance;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  distance = ultrasonic.read(); //不加參數就是輸出CM，可用read(INC)輸出英寸
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(200); //每次間格0.2秒
}
