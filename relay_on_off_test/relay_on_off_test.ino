int delayTime = 20000;
void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
}

void loop() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  delay(delayTime);
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  delay(delayTime);
}
