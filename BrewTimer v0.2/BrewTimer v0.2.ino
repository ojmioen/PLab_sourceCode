#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void(*resetFunc) (void) = 0;

const byte buttons[] = { 8, 9, 10, 11, 12 }; // choose, left, down, up, right
const char btnNames[] = { 'C', 'L', 'D', 'U', 'R' };
const byte numberOfBtns = 5;
const byte ledPin = A0;
byte btnStates[] = { 0, 0, 0, 0, 0 };
byte prevBtnStates[] = { 0, 0, 0, 0, 0 };
const byte speakerPin = A5;


const byte maxTime = 255;
const byte minTime = 0;
byte setTimerVal = 0;
byte displayedTimerVal = 0;
byte timerVal;
boolean timerStarted = false;
boolean timerEnded = false;

boolean inStartScreen = true;
boolean inMainMenu = true;
boolean inSetMashTimeMenu = false;
boolean inSetBoilTimeMenu = false;
boolean inSetHopAddTimesMenu = false;
boolean inTimerStartMenu = false;
boolean inTimerMenu = false;

byte mainMenuPage = 0; // 0=SetMashTime, 1=SetBoilTime, 2=SetHopAddTimes, 3=TimerStartMenu
const byte numOfMainMenuPages = 4;
int hopAddTimes[];
hopAddTimes


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
	pinMode(ledPin, OUTPUT);
	pinMode(speakerPin, OUTPUT);
	delay(1000);
	updateLcd();
}

void loop()
{
	unsigned long currMillis = millis();
	if (timerStarted) {
		if (timerVal == 0) {
			timerEnded = true;
			updateLcd();
			timerEnd();
		}
		else if (currMillis - prevMillis > refreshInterval) {
			timerVal--;
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
	if (inMainMenu) {
		lcd.print("MAIN MENU");
		lcd.setCursor(0, 1);
		switch (mainMenuPage) {
		case 0:
			lcd.print("Set timer");
			return;
		case 1:
			lcd.print("Start timer");
			return;
		}
	}
	else if (inTimerSetMenu) {
		lcd.print("Set value: ");
		lcd.print(setTimerVal);
		lcd.setCursor(0, 1);
		lcd.print("New value: ");
		lcd.print(displayedTimerVal);
		return;
	}
	else if (inTimerStartMenu) {
		lcd.print("Press C to start");
		lcd.setCursor(0, 1);
		lcd.print("timer for ");
		lcd.print(setTimerVal);
		lcd.print(" s");
		return;
	}
	else if (inTimerMenu) {
		if (!timerEnded) {
			lcd.print("Remaining time:");
			lcd.setCursor(0, 1);
			lcd.print(timerVal);
			lcd.print(" seconds");
			return;
		}
		else {
			lcd.print("Timer has ended.");
			return;
		}
	}
	return;
}

void handleChBtnClick() {
	if (inMainMenu) {
		inMainMenu = false;
		switch (mainMenuPage) {
		case 0:
			inTimerSetMenu = true;
			break;
		case 1:
			inTimerStartMenu = true;
			break;
		}
	}
	else if (inTimerSetMenu) {
		setTimerVal = displayedTimerVal;
	}
	else if (inTimerStartMenu) {
		inTimerStartMenu = false;
		inTimerMenu = true;
		timerStarted = true;
		timerVal = setTimerVal;
		prevMillis = millis();
	}
	updateLcd();
	return;
}

void handleUpBtnClick() {
	if (inTimerSetMenu) {
		if (displayedTimerVal < maxTime) {
			displayedTimerVal++;
		}
	}
	updateLcd();
	return;
}

void handleDownBtnClick() {
	if (inTimerSetMenu) {
		if (displayedTimerVal > minTime) {
			displayedTimerVal--;
		}
	}
	updateLcd();
	return;
}

void handleLeftBtnClick() {
	if (inMainMenu) {
		mainMenuPage = (mainMenuPage + (numOfMainMenuPages - 1)) % numOfMainMenuPages;
	}
	else if (inTimerSetMenu) {
		inTimerSetMenu = false;
		displayedTimerVal = setTimerVal;
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
		if (!(inTimerSetMenu && (i == 2 || i == 3))) {
			prevBtnStates[i] = btnStates[i];
		}
	}
	return pressedBtn;
}

void timerEnd() {
	for (int i = 0; i < 10; i++) {
		digitalWrite(ledPin, HIGH);
		tone(speakerPin, 880, 500);
		delay(500);
		digitalWrite(ledPin, LOW);
		delay(500);
	}
	resetFunc();
}