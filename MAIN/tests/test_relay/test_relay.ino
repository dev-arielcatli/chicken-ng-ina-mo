void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(7, LOW);
  delay(10000);
  digitalWrite(7, HIGH);
  delay(5000);
}
