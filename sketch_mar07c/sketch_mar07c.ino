char btVal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    btVal = Serial.read();
    Serial.println(btVal);
  }
  delay(40);
}
