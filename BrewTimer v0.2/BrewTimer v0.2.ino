#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void(*resetFunc) (void) = 0;

const byte buttons[] = { 8, 9, 10, 11, 12 }; // choose, left, down, up, right
const char btnNames[] = { 'C', 'L', 'D', 'U', 'R' };
const byte numberOfBtns = 5;
const byte ledPins[] = { A0, A1, A2 };
const byte numberOfLeds = 3;
byte btnStates[] = { 0, 0, 0, 0, 0 };
byte prevBtnStates[] = { 0, 0, 0, 0, 0 };
const byte speakerPin = A5;


const byte maxTime = 255;
const byte minTime = 0;
byte setMashTime = 0;
byte dispMashTime = 0;
byte mashTimeCounter;
byte setBoilTime = 0;
byte dispBoilTime = 0;
byte boilTimeCounter;
boolean mashTimerStarted = false;
boolean mashTimerEnded = false;
boolean boilTimerStarted = false;
boolean boilTimerEnded = false;

boolean inStartScreen = true;
boolean inMainMenu = false;
boolean inSetMashTimeMenu = false;
boolean inSetBoilTimeMenu = false;
boolean inSetHopAddTimesMenu = false;
boolean inTimerStartMenu = false;
boolean inMashTimerMenu = false;
boolean inBoilTimerMenu = false;

byte mainMenuPage = 0; // 0=SetMashTime, 1=SetBoilTime, 2=SetHopAddTimes, 3=TimerStartMenu
String mmPages[] = { "Set mash time", "Set boil time", "Set hop add times", "Start timer" };
const byte numOfMainMenuPages = 4;
byte hopAddTimes[10];
byte numberOfHopAddTimes = 0;

unsigned long prevMillis = 0;
int refreshInterval = 1000; // 1 second

void setup()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.home();
	lcd.print("Brew timer v0.2");
	lcd.setCursor(0, 1);
	lcd.print("Loading...");
	Serial.begin(9600);
	for (int i = 0; i < numberOfBtns; i++) {
		pinMode(buttons[i], INPUT);
	}
	for (int i = 0; i < numberOfLeds; i++) {
		pinMode(ledPins[i], OUTPUT);
	}
	pinMode(speakerPin, OUTPUT);
	delay(1000);
	updateLcd();
}

void loop()
{
	unsigned long currMillis = millis();
	if (mashTimerStarted && !mashTimerEnded) {
		if (mashTimeCounter == 0) {
			mashTimerEnded = true;
			updateLcd();
			timerEnd();
		}
		else if (currMillis - prevMillis > refreshInterval) {
			mashTimeCounter--;
			prevMillis = currMillis;
			updateLcd();
		}
	}
	else if (boilTimerStarted && !boilTimerEnded) {
		if (boilTimeCounter == 0) {
			boilTimerEnded = true;
			updateLcd();
			timerEnd();
		}
		else if (currMillis - prevMillis > refreshInterval) {
			boilTimeCounter--;
			prevMillis = currMillis;
			updateLcd();
		}
	}
	char pressedBtn = buttonPressed();

	switch (pressedBtn) {
	case 'C':
		Serial.println(pressedBtn);
		handleChBtnClick();
		break;
	case 'U':
		Serial.println(pressedBtn);
		handleUpBtnClick();
		break;
	case 'D':
		Serial.println(pressedBtn);
		handleDownBtnClick();
		break;
	case 'L':
		Serial.println(pressedBtn);
		handleLeftBtnClick();
		break;
	case 'R':
		Serial.println(pressedBtn);
		handleRightBtnClick();
		break;
	}

	delay(100);
}

void updateLcd() {
	lcd.clear();
	lcd.home();
	if (inStartScreen) {
		lcd.print("BrewTimer v0.2");
		lcd.setCursor(0, 1);
		lcd.print(">press to start");
		return;
	}
	else if (inMainMenu) {
		lcd.print("MAIN MENU");
		lcd.setCursor(0, 1);
		lcd.print(mmPages[mainMenuPage]);
		return;
	}
	else if (inSetMashTimeMenu) {
		lcd.print("Mash time: ");
		lcd.print(setMashTime);
		lcd.setCursor(0, 1);
		lcd.print("New time: ");
		lcd.print(dispMashTime);
		return;
	}
	else if (inSetBoilTimeMenu) {
		lcd.print("Boil time: ");
		lcd.print(setBoilTime);
		lcd.setCursor(0, 1);
		lcd.print("New time: ");
		lcd.print(dispBoilTime);
		return;
	}
	else if (inSetHopAddTimesMenu) {
		lcd.print("This feature is");
		lcd.setCursor(0, 1);
		lcd.print("not yet added");
		return;
	}
	else if (inTimerStartMenu) {
		lcd.print("Press to start");
		lcd.setCursor(0, 1);
		lcd.print("brew day");
		return;
	}
	else if (inMashTimerMenu) {
		if (!mashTimerEnded) {
			lcd.print("Mash time left:");
			lcd.setCursor(0, 1);
			lcd.print(mashTimeCounter);
			lcd.print(" seconds");
			return;
		}
		else {
			lcd.print("Mashing is done");
			lcd.setCursor(0, 1);
			lcd.print("Press for boil");
			return;
		}
	}
	else if (inBoilTimerMenu) {
		if (!boilTimerEnded) {
			lcd.print("Boil time left:");
			lcd.setCursor(0, 1);
			lcd.print(boilTimeCounter);
			lcd.print(" seconds");
			return;
		}
		else {
			lcd.print("Boil is done");
			lcd.setCursor(0, 1);
			lcd.print("Press to exit");
		}
	}
	return;
}

void handleChBtnClick() {
	if (inStartScreen) {
		inStartScreen = false;
		inMainMenu = true;
	}
	else if (inMainMenu) {
		inMainMenu = false;
		switch (mainMenuPage) {
		case 0:
			inSetMashTimeMenu = true;
			break;
		case 1:
			inSetBoilTimeMenu = true;
			break;
		case 2:
			inSetHopAddTimesMenu = true;
			break;
		case 3:
			inTimerStartMenu = true;
			break;
		}
	}
	else if (inSetMashTimeMenu) {
		setMashTime = dispMashTime;
	}
	else if (inSetBoilTimeMenu) {
		setBoilTime = dispBoilTime;
	}
	else if (inSetHopAddTimesMenu) {
		Serial.println("hop add times");
	}
	else if (inTimerStartMenu) {
		inTimerStartMenu = false;
		inMashTimerMenu = true;
		mashTimeCounter = setMashTime;
		boilTimeCounter = setBoilTime;
		mashTimerStarted = true;
		prevMillis = millis();
	}
	else if (inMashTimerMenu) {
		if (mashTimerEnded) {
			inMashTimerMenu = false;
			inBoilTimerMenu = true;
			boilTimerStarted = true;
			prevMillis = millis();
		}
	}
	else if (inBoilTimerMenu) {
		if (boilTimerEnded) {
			resetFunc();
		}
	}
	updateLcd();
	return;
}

void handleUpBtnClick() {
	if (inSetMashTimeMenu) {
		if (dispMashTime < maxTime) {
			dispMashTime++;
		}
	}
	else if (inSetBoilTimeMenu) {
		if (dispBoilTime < maxTime) {
			dispBoilTime++;
		}
	}
	updateLcd();
	return;
}

void handleDownBtnClick() {
	if (inSetMashTimeMenu) {
		if (dispMashTime > minTime) {
			dispMashTime--;
		}
	}
	else if (inSetBoilTimeMenu) {
		if (dispBoilTime > minTime) {
			dispBoilTime--;
		}
	}
	updateLcd();
	return;
}

void handleLeftBtnClick() {
	if (inMainMenu) {
		mainMenuPage = (mainMenuPage + (numOfMainMenuPages - 1)) % numOfMainMenuPages;
	}
	else if (inSetMashTimeMenu) {
		inSetMashTimeMenu = false;
		dispMashTime = setMashTime;
		inMainMenu = true;
	}
	else if (inSetBoilTimeMenu) {
		inSetBoilTimeMenu = false;
		dispBoilTime = setBoilTime;
		inMainMenu = true;
	}
	else if (inSetHopAddTimesMenu) {
		inSetHopAddTimesMenu = false;
		inMainMenu = true;
	}
	else if (inTimerStartMenu) {
		inTimerStartMenu = false;
		inMainMenu = true;
	}
	updateLcd();
	return;
}

void handleRightBtnClick() {
	if (inMainMenu) {
		mainMenuPage = (mainMenuPage + 1) % numOfMainMenuPages;
	}
	updateLcd();
	return;
}

char buttonPressed() {
	char pressedBtn = '\0';
	for (int i = numberOfBtns - 1; i >-1; i--) {
		btnStates[i] = digitalRead(buttons[i]);
		if (btnStates[i] == HIGH && btnStates[i] != prevBtnStates[i]) {
			pressedBtn = btnNames[i];
		}
		if (!((inSetMashTimeMenu || inSetBoilTimeMenu) && (i == 2 || i == 3))) {
			prevBtnStates[i] = btnStates[i];
		}
	}
	return pressedBtn;
}

void timerEnd() {
	for (int i = 0; i < 6; i++) {
		digitalWrite(ledPins[i%3], HIGH);
		tone(speakerPin, 880, 500);
		delay(500);
		digitalWrite(ledPins[i%3], LOW);
		delay(500);
	}
}