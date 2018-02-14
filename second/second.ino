const int greenLedPin = 2;
const int yellowLedPin = 3;
const int redLedPin = 4;

void setup() {
  // put your setup code here, to run once:
  for (int i = 2; i < 5; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 2; i < 5; i++) {
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
    delay(1000);
  }
  for (int i = 0; i < 3; i++) {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    delay(500);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    delay(500);
  }
}
