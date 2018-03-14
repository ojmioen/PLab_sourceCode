#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const byte buttons[] = { 8, 9, 10, 11, 12 }; // choose, left, down, up, right
// const String btnNames[] = { "Choose", "Left", "Down", "Up", "Right" };
const byte numberOfButtons = 5;
const byte ledPin = 13;
byte ledState = 0;
byte btnStates[] = { 0, 0, 0, 0, 0 };
byte prevBtnStates[] = { 0, 0, 0, 0, 0 };
byte menuIndex = 0;
byte blinkState = 0;
byte ledBlinkState = 0;

unsigned long prevMillis = 0;
long interval = 500;

void setup()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.home();
	lcd.print("LED ON/OFF");
	printMenu(0);
	Serial.begin(9600);
	for (int i = 0; i < numberOfButtons; i++) {
		pinMode(buttons[i], INPUT);
	}
	pinMode(ledPin, OUTPUT);
}

void loop()
{
	unsigned long currMillis = millis();
	if (blinkState) {
		if (currMillis - prevMillis > interval) {
		Serial.println("Blink");
			prevMillis = currMillis;

			if (!ledBlinkState)
				ledBlinkState = HIGH;
			else
				ledBlinkState = LOW;
			digitalWrite(ledPin, ledBlinkState);
		}
	}
	byte pressedButton = buttonPressed();
	
	switch (pressedButton) {
	case 0: // choose
		Serial.println("OK");
		handleChBtnClick(menuIndex);
		break;
	case 1: // left
		menuIndex = (menuIndex + 2) % 3;
		Serial.println(menuIndex);
		printMenu(menuIndex);
		break;
	case 4: // right
		menuIndex = (menuIndex + 1) % 3;
		Serial.println(menuIndex);
		printMenu(menuIndex);
		break;
	default:
		break;
	}


	delay(100);
}

void printMenu(byte index) {
	lcd.setCursor(0, 1);
	lcd.print("      ");
	lcd.setCursor(0, 1);
	switch (index) {
	case 0:
		lcd.print("ON");
		if (ledState)
			lcd.print("*");
		return;
	case 1:
		lcd.print("OFF");
		if (!ledState && !blinkState)
			lcd.print("*");
		return;
	case 2:
		lcd.print("BLINK");
		if (blinkState)
			lcd.print("*");
	default:
		return;
	}
}

byte buttonPressed() {
	byte pressedBtn = numberOfButtons;
	for (int i = numberOfButtons-1; i >-1; i--) {
		btnStates[i] = digitalRead(buttons[i]);
		if (btnStates[i] == HIGH && btnStates[i] != prevBtnStates[i]) {
			pressedBtn = i;
		}
		prevBtnStates[i] = btnStates[i];
	}
	return pressedBtn;
}

void handleChBtnClick(byte index) {
	switch (index) {
	case 0:
		if (!ledState) {
			digitalWrite(ledPin, HIGH);
			ledState = HIGH;
			blinkState = LOW;
			printMenu(menuIndex);
		}
		return;
	case 1:
		if (ledState || blinkState) {
			digitalWrite(ledPin, LOW);
			ledState = LOW;
			blinkState = LOW;
			printMenu(menuIndex);
		}
		return;
	case 2:
		if (ledState) {
			ledState = LOW;
			digitalWrite(ledPin, LOW);
		}
		if (!blinkState) {
			blinkState = HIGH;
			printMenu(menuIndex);
		}
		return;
	}
	return;
}