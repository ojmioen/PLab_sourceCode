#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

boolean successRead;
byte readCard[4];
byte lastReadCard[4];

const int buttonPin = A0; // Resistor ladder
const int potPin = A1;    // Pitch shifter
const int speakerPin = 3;

byte studentCard[] = {206, 98, 229, 199};
byte rfidTag[] = {129, 27, 229, 43};

void setup()
{
	Serial.begin(9600);
	pinMode(speakerPin, OUTPUT);
	SPI.begin();
	mfrc522.PCD_Init();
}

void loop()
{
	successRead = getID();
	if (successRead) {
		printID(readCard);
		for (int i = 0; i < 4; i++) {
			lastReadCard[i] = readCard[i];
		}	
	}

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
  /* add main program code here */

}

boolean getID() {
	for (int i = 0; i < 4; i++) {  // 
		readCard[i] = 0;
	};
	// Getting ready for Reading PICCs
	if (!mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
		return false;
	}
	if (!mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
		return false;
	}
	for (int i = 0; i < 4; i++) {  // 
		readCard[i] = mfrc522.uid.uidByte[i];
	}
	mfrc522.PICC_HaltA(); // Stop reading
	return true;
}

void printID(byte a[]) {
	for (int i = 0; i < 4; i++) {
		Serial.print(a[i]);
		Serial.print(" ");
	}
	Serial.println();
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
	int shiftedPitch = pitch + pitch * (5 * potVal / 1024);
		if (readCardMatches(studentCard)) {
			shiftedPitch = shiftedPitch / 2;
		}
	return shiftedPitch;
}

boolean readCardMatches(byte a[]) {
	for (int i = 0; i < 4; i++) {
		if (a[i] != lastReadCard[i]) {
			return false;
		}
	}
	return true;
}
