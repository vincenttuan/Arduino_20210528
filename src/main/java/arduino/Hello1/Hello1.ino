int i = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Call setup ... ");

}

void loop() {
  i++;
  if(i % 2 == 0) {
      Serial.print("Call loop i = ");
      Serial.println(i);
  }
  delay(500);
}
