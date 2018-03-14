#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const byte buttons[] = { 8, 9, 10, 11, 12 }; // choose, left, down, up, right
const char btnNames[] = { 'C', 'L', 'D', 'U', 'R' };
const byte numberOfButtons = 5;
byte btnStates[] = { 0, 0, 0, 0, 0 };
byte prevBtnStates[] = { 0, 0, 0, 0, 0 };

const byte maxTime = 255;
const byte minTime = 0;
byte chosenTime = 0;
byte displayedTime = 0;

void setup()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.home();
	for (int i = 0; i < numberOfButtons; i++) {
		pinMode(buttons[i], INPUT);
	}
	lcd.print("Loading...");
	delay(200);
	updateLcd();
	Serial.begin(9600);
}

void loop()
{
	char pressedBtn = buttonPressed();

	switch (pressedBtn) {
	case 'C':
		Serial.println(pressedBtn);
		chosenTime = displayedTime;
		updateLcd();
		break;
	case 'U':
		Serial.println(pressedBtn);
		if (displayedTime < maxTime) {
			displayedTime++;
		}
		updateLcd();
		break;
	case 'D':
		Serial.println(pressedBtn);
		if (displayedTime > minTime) {
			displayedTime--;
		}
		updateLcd();
		break;
	}

	delay(100);
}

char buttonPressed() {
	char pressedBtn = '\0';
	for (int i = numberOfButtons - 1; i >-1; i--) {
		btnStates[i] = digitalRead(buttons[i]);
		if (btnStates[i] == HIGH && btnStates[i] != prevBtnStates[i]) {
			pressedBtn = btnNames[i];
		}
		if (i != 2 && i != 3) {
			prevBtnStates[i] = btnStates[i];
		}
	}
	return pressedBtn;
}

void updateLcd() {
	lcd.clear();
	lcd.home();
	lcd.print("Set value: ");
	lcd.print(chosenTime);
	lcd.setCursor(0, 1);
	lcd.print("New value: ");
	lcd.print(displayedTime);
	return;
}