const int trigPin = 7;
const int echoPin = 8;
const int speakerPin = 3;

const float distHigh = 5;
const float distLow = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  float distance = getDistance();
  Serial.print(distance);
  Serial.print("\t");
  int pitch = map(distance, distLow, distHigh, 100, 3000);
  Serial.println(pitch);
  tone(speakerPin, pitch, 100);
  delay(100);
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.00017;

  return distance;
}

