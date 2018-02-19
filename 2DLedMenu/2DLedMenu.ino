#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const byte ledPins[] = { A0, A1, A2 }; //r, g, b
const byte numofledpins = 3;
const byte buttons[] = { 8, 9, 10, 11, 12 }; // choose, left, down, up, right
const char btnNames[] = { 'C', 'L', 'D', 'U', 'R' };
const byte numberOfButtons = 5;
byte btnStates[] = { 0, 0, 0, 0, 0 };
byte prevBtnStates[] = { 0, 0, 0, 0, 0 };

boolean inMainMenu = true;
boolean inRMenu = false;
boolean inGMenu = false;
boolean inBMenu = false;

byte mainMenuPage = 0; // 0=RED, 1=GREEN, 2=BLUE
const byte numOfMainPages = 3;
byte rMenuPage = 0;
byte gMenuPage = 0;
byte bMenuPage = 0;
const byte numOfSubPages = 2;

boolean rLedOn = false;
boolean gLedOn = false;
boolean bLedOn = false;

void setup() {
	for (int i = 0; i < numofledpins; i++) {
		pinMode(ledPins[i], OUTPUT);
	}
	for (int i = 0; i < numberOfButtons; i++) {
		pinMode(buttons[i], INPUT);
	}
	lcd.begin(16, 2);
	lcd.clear();
	lcd.home();
	lcd.print("Loading...");
	printMainMenu();
	Serial.begin(9600);
}

void loop()
{
	char pressedButton = buttonPressed();

	switch (pressedButton) {
	case 'C': // choose - enter menu if in main menu, toggle setting if not
		Serial.println(pressedButton);
		handleChBtnClick();
		break;
	case 'L': // left - scroll left in menu
		Serial.println(pressedButton);
		if (inMainMenu) {
			mainMenuPage = (mainMenuPage + 2) % 3;
			printMainMenu();
		}
		else {
			changeSubMenu();
			printSubMenu();
		}
		break;
	case 'R': // right - scroll right in menu
		Serial.println(pressedButton);
		if (inMainMenu) {
			mainMenuPage = (mainMenuPage + 1) % 3;
			printMainMenu();
		}
		else {
			changeSubMenu();
			printSubMenu();
		}
		break;
	case 'D': // down - if not in main menu: return to main menu
		Serial.println(pressedButton);
		handleDownBtnClick();
		break;
	default:
		break;
	}


	delay(100);
}

void printMainMenu() {
	lcd.clear();
	lcd.home();
	lcd.print("RGB MAIN MENU");
	lcd.setCursor(0, 1);
	switch (mainMenuPage) {
	case 0:
		lcd.print(">Red");
		return;
	case 1:
		lcd.print(">Green");
		return;
	case 2:
		lcd.print(">Blue");
		return;
	default:
		Serial.println("Invalid main menu page value");
		return;
	}
	return;
}

void printSubMenu() {
	lcd.clear();
	lcd.home();
	lcd.print("SUB MENU: ");
	if (inRMenu) {
		lcd.print("RED");
		lcd.setCursor(0, 1);
		switch (rMenuPage) {
		case 0:
			lcd.print("On");
			if (rLedOn)
				lcd.print("*");
			break;
		case 1:
			lcd.print("Off");
			if (!rLedOn)
				lcd.print("*");
			break;
		}
	}
	else if (inGMenu) {
		lcd.print("GREEN");
		lcd.setCursor(0, 1);
		switch (gMenuPage) {
		case 0:
			lcd.print("On");
			if (gLedOn)
				lcd.print("*");
			break;
		case 1:
			lcd.print("Off");
			if (!gLedOn)
				lcd.print("*");
			break;
		}
	}
	else if (inBMenu) {
		lcd.print("BLUE");
		lcd.setCursor(0, 1);
		switch (bMenuPage) {
		case 0:
			lcd.print("On");
			if (bLedOn)
				lcd.print("*");
			break;
		case 1:
			lcd.print("Off");
			if (!bLedOn)
				lcd.print("*");
			break;
		}
	}
	return;
}

void changeSubMenu() {
	if (inRMenu) {
		rMenuPage = (rMenuPage + 1) % 2;
	}
	else if (inGMenu) {
		gMenuPage = (gMenuPage + 1) % 2;
	}
	else if (inBMenu) {
		bMenuPage = (bMenuPage + 1) % 2;
	}
	return;
}

char buttonPressed() {
	char pressedBtn = '\0';
	for (int i = numberOfButtons - 1; i >-1; i--) {
		btnStates[i] = digitalRead(buttons[i]);
		if (btnStates[i] == HIGH && btnStates[i] != prevBtnStates[i]) {
			pressedBtn = btnNames[i];
		}
		prevBtnStates[i] = btnStates[i];
	}
	return pressedBtn;
}

void handleChBtnClick() {
	if (inMainMenu) {
		inMainMenu = false;
		switch (mainMenuPage) {
		case 0:
			inRMenu = true;
			rMenuPage = 0;
			break;
		case 1:
			inGMenu = true;
			gMenuPage = 0;
			break;
		case 2:
			inBMenu = true;
			bMenuPage = 0;
			break;
		default:
			Serial.println("Invalid main menu page value");
		}
	}
	else {
		if (inRMenu) {
			switch (rMenuPage) {
			case 0:
				digitalWrite(ledPins[0], HIGH);
				rLedOn = true;
				break;
			case 1:
				digitalWrite(ledPins[0], LOW);
				rLedOn = false;
				break;
			}
		}
		else if (inGMenu) {
			switch (gMenuPage) {
			case 0:
				digitalWrite(ledPins[1], HIGH);
				gLedOn = true;
				break;
			case 1:
				digitalWrite(ledPins[1], LOW);
				gLedOn = false;
				break;
			}
		}
		else if (inBMenu) {
			switch (bMenuPage) {
			case 0:
				digitalWrite(ledPins[2], HIGH);
				bLedOn = true;
				break;
			case 1:
				digitalWrite(ledPins[2], LOW);
				bLedOn = false;
				break;
			}
		}
	}
	printSubMenu();
	return;
}

void handleDownBtnClick() {
	if (!inMainMenu) {
		inRMenu = false;
		inBMenu = false;
		inGMenu = false;
		rMenuPage = 0;
		gMenuPage = 0;
		bMenuPage = 0;
		inMainMenu = true;
		printMainMenu();
	}
}