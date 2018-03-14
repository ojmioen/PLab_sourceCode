#include <ZumoReflectanceSensorArray.h>

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors;

void setup()
{
   sensors.init();
   Serial.begin(9600);
}

void loop()
{
  sensors.read(sensor_values);
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensor_values[i]);
  }
  delay(1000);
}

