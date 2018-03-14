#include <NewPing.h>
#include <PLab_ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>

#define FRONT_PING_PIN 2
#define REAR_PING_PIN 6
#define MAX_SONAR_DIST 40
#define LED 13

#define REVERSE_SPEED     250 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     140
#define FORWARD_SPEED_B   350
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     250 // ms
#define QTR_THRESHOLD     800

//int forwardSpeed = FORWARD_SPEED;


ZumoMotors motors;
NewPing frontSonar(FRONT_PING_PIN, FRONT_PING_PIN, MAX_SONAR_DIST);
NewPing rearSonar(REAR_PING_PIN, REAR_PING_PIN, MAX_SONAR_DIST);

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors;
Pushbutton button(ZUMO_BUTTON);

float frontDist = 0;
float rearDist = 0;

//char bluetoothVal;
boolean beastMode = true;
//boolean prevBeastMode = false;

void setup() {
  Serial.begin(9600);
  sensors.init();
  button.waitForButton();
}

void loop() {
  pollSensor();
  //pullBluetooth();
  sensors.read(sensor_values);
  handleSensors();
  
  delay(50);
}

void pollSensor() {
  frontDist = frontSonar.ping_cm();
  rearDist = rearSonar.ping_cm();
  Serial.print("Front distance: ");
  Serial.println(frontDist);
  Serial.print("Rear distance: ");
  Serial.println(rearDist);
  
}

void handleSensors() {
  int forwardSpeed;
  if (beastMode) {
    forwardSpeed = FORWARD_SPEED_B;
  }
  else {
    forwardSpeed = FORWARD_SPEED;
  }
  //Arrray sensors
  if (sensor_values[0] < QTR_THRESHOLD){
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
  }
  else if (sensor_values[5] < QTR_THRESHOLD){
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
  }

  //Front sensor
  else if (frontDist > 0 && frontDist < 20) {
    motors.setSpeeds(400, 400);
  }
  //Rear sensor
  else if (rearDist > 0 && rearDist < 15) {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION - 80);
  }
  else {
    motors.setSpeeds(forwardSpeed, forwardSpeed);
  }
}

//void pullBluetooth() {
//  if (Serial.available()) {
//    bluetoothVal = Serial.read();
//    Serial.println(bluetoothVal);
//    if (bluetoothVal == 'b') {
//      digitalWrite(LED, HIGH);
//      prevBeastMode = beastMode;
//      beastMode = true;
//    }
//    else if (bluetoothVal == 'p') {
//      digitalWrite(LED, LOW);
//      prevBeastMode = beastMode;
//      beastMode = false;
//    }
//  }
//}

