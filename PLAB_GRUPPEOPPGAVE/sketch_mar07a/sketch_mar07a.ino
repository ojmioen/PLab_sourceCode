#include <NewPing.h>
#include <PLab_ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <SoftwareSerial.h>

//SensorPins
#define FRONT_PING_PIN 2
#define RIGHT_PING_PIN 6
#define LEFT_PING_PIN 3
#define REAR_PING_PIN A1

//Bluetooth pins
#define TX_PIN A4
#define RX_PIN A5

//Speeds
#define REVERSE_SPEED     250 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     350
#define FORWARD_SPEED_B   400

//Durations
#define REVERSE_DURATION  250 // ms
#define TURN_DURATION     250 // ms

#define MAX_SONAR_DIST 40
#define LED 13
#define QTR_THRESHOLD     800

//int forwardSpeed = FORWARD_SPEED;

void(*resetFunc) (void) = 0;

ZumoMotors motors;
SoftwareSerial btSerial(TX_PIN, RX_PIN);

//Sonar setup
NewPing frontSonar(FRONT_PING_PIN, FRONT_PING_PIN, MAX_SONAR_DIST);
NewPing rightSonar(RIGHT_PING_PIN, RIGHT_PING_PIN, MAX_SONAR_DIST);
NewPing leftSonar(LEFT_PING_PIN, LEFT_PING_PIN, MAX_SONAR_DIST);
NewPing rearSonar(REAR_PING_PIN, REAR_PING_PIN, MAX_SONAR_DIST);

float frontDist = 0;
float rightDist = 0;
float leftDist = 0;
float rearDist = 0;

//Lightarray setup
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
ZumoReflectanceSensorArray sensors;

Pushbutton button(ZUMO_BUTTON);



char bluetoothMsg;
boolean beastMode = false;
//boolean prevBeastMode = false;


void setup() {
  //Serial.begin(9600);
  btSerial.begin(9600);
  sensors.init();
  button.waitForButton();
}


void loop() {
  pollSensor();
  pullBluetooth();
  sensors.read(sensor_values);
  handleSensors();
  
  delay(30);
}


void pollSensor() {
  frontDist = frontSonar.ping_cm();
  rightDist = rightSonar.ping_cm();
  leftDist = leftSonar.ping_cm();
  rearDist = rearSonar.ping_cm();
  //Serial.print("Front distance: ");
  //Serial.println(frontDist);
  //Serial.print("Right distance: ");
  //Serial.println(rightDist);
  
}


int lightSense() {
  if (sensor_values[0] < QTR_THRESHOLD){
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    return 1;
  }
  else if (sensor_values[5] < QTR_THRESHOLD){
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    return 1;
  }
  return 0;
}


int delayHandler(int delaySek) {
  for(int i=0; i < delaySek; i+=5) {
    sensors.read(sensor_values);
    if (lightSense() == 1) {
      return 1 ;
    }
    delay(5);
  }
  return 0;
}


void handleSensors() {
  int forwardSpeed;
  if (beastMode) {
    forwardSpeed = FORWARD_SPEED_B;
  }
  else {
    forwardSpeed = FORWARD_SPEED;
  }
  
  //Passer på å ikke kjøre ut
  lightSense();

  //Front sensor
  if (frontDist > 0 && frontDist < 20) {
    motors.setSpeeds(400, 400);
  }

  //Front flick (BETA)
  else if (frontDist > 0 && frontDist <= 6) {
    motors.setSpeeds(-400, -100);
    delay(450);
    motors.setSpeeds(400, 400);
    delay(TURN_DURATION - 150);
  }

  
  //Right sensor
  else if (rightDist > 0 && rightDist < 15) {
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION - 120);
  }
  //Left sensor
  else if (leftDist > 0 && leftDist < 15) {
    motors.setSpeeds(TURN_SPEED, TURN_SPEED);
    //motors.setSpeeds(80, 80);

    if (delayHandler(TURN_DURATION - 50) == 0) {
    motors.setSpeeds(-400, -400);
    delay(180); 
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION - 60);
    }
  }
  //Rear sensor
  else if (rearDist > 0 && rearDist < 15) {
    motors.setSpeeds(TURN_SPEED, TURN_SPEED - 200);
    delayHandler(TURN_DURATION);
  }
  //Else
  else {
    motors.setSpeeds(forwardSpeed, forwardSpeed);
  }
}

void pullBluetooth() {
  if (btSerial.available()) {
    bluetoothMsg = btSerial.read();
    if (bluetoothMsg == 'b') {
      beastMode = true;
    } else if (bluetoothMsg == 'p') {
      beastMode = false;
    } else if (bluetoothMsg == 'k') {
      motors.setSpeeds(0, 0);
      resetFunc();
    }
  }
}

