const int blinkPin = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(blinkPin, OUTPUT);
  digitalWrite(blinkPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(blinkPin, HIGH);
  delay(1000);
  digitalWrite(blinkPin, LOW);
  delay(500);
}
