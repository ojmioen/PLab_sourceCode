#include <Servo.h>

const int trafRedLight = 10;
const int trafYelLight = 9;
const int trafGreLight = 8;
const int pedRedLight = 7;
const int pedGreLight = 6;
const int buttonPin = 2;
const int servoPin = 3;
int switchState = 0;
int prevSwitchState = 0;

Servo barrierServo;

void setup() {
  for (int i = pedGreLight; i < trafRedLight+1; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(buttonPin, INPUT);
  digitalWrite(trafGreLight, HIGH);
  digitalWrite(pedRedLight, HIGH);
  barrierServo.attach(servoPin);
  barrierServo.write(0);
}

void loop() {
  switchState = digitalRead(buttonPin);
  if (switchState != prevSwitchState && switchState == 1) {
    delay(1000);
    digitalWrite(trafGreLight, LOW);
    digitalWrite(trafYelLight, HIGH);
    delay(1000);
    digitalWrite(trafYelLight, LOW);
    digitalWrite(trafRedLight, HIGH);
    delay(2000);
    // Start of pedestrian crossing
    digitalWrite(pedRedLight, LOW);
    digitalWrite(pedGreLight, HIGH);
    barrierServo.write(80);
    delay(5000);
    digitalWrite(pedGreLight, LOW);
    digitalWrite(pedRedLight, HIGH);
    barrierServo.write(0);
    // End of pedestrian crossing
    delay(2000);
    digitalWrite(trafYelLight, HIGH);
    delay(1000);
    digitalWrite(trafRedLight, LOW);
    digitalWrite(trafYelLight, LOW);
    digitalWrite(trafGreLight, HIGH);
  }
  
  prevSwitchState = switchState;
}
