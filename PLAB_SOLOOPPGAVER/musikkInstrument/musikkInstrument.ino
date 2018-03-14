// Musikkinstrument - oving 2 plab
const int buttonPin = A0; // Resistor ladder
const int potPin = A1;    // Pitch shifter
const int speakerPin = 3;



void setup() {
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(buttonPin);
  int potRead = analogRead(potPin);
  int pitch = getPitch(reading);
  int shiftedPitch = shiftPitch(pitch, potRead);
  Serial.println(shiftedPitch);
  if (pitch == 0) {
    noTone(speakerPin);
  }
  else {
    tone(speakerPin, shiftedPitch);
  }
  delay(100);
}

int getPitch(int reading) {
	int tones[] = { 0, 262, 294, 330, 349, 392, 440, 493, 523, 587 };
	// tones = {0, C4, D4, E4, F4, G4, A4, B4, C5, D5}

  if (reading < 200) {
	  return tones[0];
  }
  else if (reading < 260) {
	  return tones[1]; // C4
  }
  else if (reading < 342) {
	  return tones[2]; // D4
  }
  else if (reading < 470) {
	  return tones[3]; // E4
  }
  else if (reading < 515) {
	  return tones[4]; // F4
  }
  else if (reading < 590) {
	  return tones[5]; // G4
  }
  else if (reading < 620) {
	  return tones[6]; // A4
  }
  else if (reading < 990) {
	  return tones[7]; // B4
  }
  else if (reading < 1010) {
	  return tones[8]; // C5
  }
  else if (reading >= 1010) {
	  return tones[9]; // D5
  }
  else {
    return 0;
  }
}

int shiftPitch(int pitch, int potVal) {
  int shiftedPitch = pitch + pitch*(5*potVal/1024);
  return shiftedPitch;
}

