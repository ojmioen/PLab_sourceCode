#include <SoftwareSerial.h>

#define TX_PIN A4
#define RX_PIN A5

SoftwareSerial bt(TX_PIN, RX_PIN);

char msg;
boolean LEDState = LOW;

void setup() {
  // put your setup code here, to run once:
  bt.begin(9600);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if (bt.available()) {
    msg = bt.read();
    Serial.println(msg);
    if (msg == 'o') {
      LEDState = !LEDState;
      digitalWrite(13, LEDState);
    }
  }
  delay(40);
}
