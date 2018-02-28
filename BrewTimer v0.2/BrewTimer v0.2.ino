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

const byte maxTime= 255;
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
boolean inHopAddSubMenu = false;
boolean inTimerStartMenu = false;
boolean inMashTimerMenu = false;
boolean inBoilTimerMenu = false;
boolean inEmptyTimersMenu = false;
boolean inHopAddFinishedMenu = false;


int hopAddTimes[10];
byte numberOfHopAddTimes = 0;
byte curNumOfHopAddTimes = 0;
byte dispNumOfHopAdds, setNumOfHopAdds = 0;
byte dispHopAddTime, setHopAddTime = 0;
const byte maxHopAdds = 10;
const byte minHopAdds = 0;



byte mainMenuPage = 0; // 0=SetMashTime, 1=SetBoilTime, 2=SetHopAddTimes, 3=TimerStartMenu
String mmPages[] = { "Set mash time", "Set boil time", "Set hop times", "Start timer" };
const byte numOfMainMenuPages = 4;

unsigned long prevMillis = 0;
unsigned int refreshInterval = 10000; // 1 minute

void setup()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.home();
	lcd.print("Brew timer v1.0");
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
			lcd.noBlink();
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
			lcd.noBlink();
			updateLcd();
			timerEnd();
		}
		else if (currMillis - prevMillis > refreshInterval) {
			boilTimeCounter--;
			prevMillis = currMillis;
			updateLcd();
			for (int i = 0; i < numberOfHopAddTimes; i++) {
				if (hopAddTimes[i] >= boilTimeCounter) {
					hopAlert(i);
				}
			}
		}
	}
	char pressedBtn = buttonPressed();

	switch (pressedBtn) {
	case 'C':
		//Serial.println(pressedBtn);
		handleChBtnClick();
		break;
	case 'U':
		//Serial.println(pressedBtn);
		handleUpBtnClick();
		break;
	case 'D':
		//Serial.println(pressedBtn);
		handleDownBtnClick();
		break;
	case 'L':
		//Serial.println(pressedBtn);
		handleLeftBtnClick();
		break;
	case 'R':
		//Serial.println(pressedBtn);
		handleRightBtnClick();
		break;
	}

	delay(100);
}

void updateLcd() {
	lcd.clear();
	lcd.home();
	if (inStartScreen) {
		lcd.print("BrewTimer v1.0");
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
		lcd.print("New time:  ");
		lcd.print(dispMashTime);
		return;
	}
	else if (inSetBoilTimeMenu) {
		lcd.print("Boil time: ");
		lcd.print(setBoilTime);
		lcd.setCursor(0, 1);
		lcd.print("New time:  ");
		lcd.print(dispBoilTime);
		return;
	}
	else if (inSetHopAddTimesMenu) {
		lcd.print("No of hop adds:");
		lcd.setCursor(0, 1);
		lcd.print("Set:");
		lcd.print(setNumOfHopAdds);
		lcd.print(" New:");
		lcd.print(dispNumOfHopAdds);
		return;
	}
	else if (inHopAddSubMenu) {
		lcd.print("Hop addition ");
		lcd.print(curNumOfHopAddTimes + 1);
		lcd.setCursor(0, 1);
		lcd.print("New:");
		lcd.print(dispHopAddTime);
		return;
	}
	else if (inHopAddFinishedMenu) {
		lcd.print(curNumOfHopAddTimes);
		lcd.print(" hop times");
		lcd.setCursor(0, 1);
		lcd.print("added.");
		return;
	}
	else if (inTimerStartMenu) {
		lcd.print("Press to start");
		lcd.setCursor(0, 1);
		lcd.print("brew day");
		return;
	}
	else if (inEmptyTimersMenu) {
		lcd.print("No mash/boil");
		lcd.setCursor(0, 1);
		lcd.print("time set.");
	}
	else if (inMashTimerMenu) {
		if (!mashTimerEnded) {
			lcd.print("Mash time left:");
			lcd.setCursor(0, 1);
			lcd.print(mashTimeCounter);
			lcd.print(" minutes");
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
			lcd.print(" minutes");
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
		setNumOfHopAdds = dispNumOfHopAdds;
	}
	else if (inHopAddSubMenu) {
		hopAddTimes[curNumOfHopAddTimes] = dispHopAddTime;
		dispHopAddTime = 0;
		curNumOfHopAddTimes++;
		if (curNumOfHopAddTimes == numberOfHopAddTimes) {
			inHopAddSubMenu = false;
			inHopAddFinishedMenu = true;
		}
	}
	else if (inHopAddFinishedMenu) {
		inHopAddFinishedMenu = false;
		inMainMenu = true;
		mainMenuPage = 3;
	}
	else if (inTimerStartMenu) {
		if (!(setMashTime == 0 || setBoilTime == 0)) {
			inTimerStartMenu = false;
			inMashTimerMenu = true;
			mashTimeCounter = setMashTime;
			boilTimeCounter = setBoilTime;
			mashTimerStarted = true;
			prevMillis = millis();
			lcd.blink();
		}
		else {
			inTimerStartMenu = false;
			inEmptyTimersMenu = true;
		}
	}
	else if (inEmptyTimersMenu) {
		inEmptyTimersMenu = false;
		mainMenuPage = 0;
		inMainMenu = true;
	}
	else if (inMashTimerMenu) {
		if (mashTimerEnded) {
			inMashTimerMenu = false;
			inBoilTimerMenu = true;
			boilTimerStarted = true;
			lcd.blink();
			prevMillis = millis();
			for (int i = 0; i < numberOfHopAddTimes; i++) {
				if (hopAddTimes[i] >= boilTimeCounter) {
					hopAlert(i);
				}
			}
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
	if (inStartScreen) {
		inStartScreen = false;
		inMainMenu = true;
	}
	else if (inSetMashTimeMenu) {
		if (dispMashTime < maxTime) {
			dispMashTime++;
		}
	}
	else if (inSetBoilTimeMenu) {
		if (dispBoilTime < maxTime) {
			dispBoilTime++;
		}
	}
	else if (inSetHopAddTimesMenu) {
		if (dispNumOfHopAdds < maxHopAdds) {
			dispNumOfHopAdds++;
		}
	}
	else if (inHopAddSubMenu) {
		byte maxHopTime;
		setBoilTime > 0 ? maxHopTime = setBoilTime : maxHopTime = maxTime;		
		if (dispHopAddTime < maxHopTime) {
			dispHopAddTime++;
		}
	}
	updateLcd();
	return;
}

void handleDownBtnClick() {
	if (inStartScreen) {
		inStartScreen = false;
		inMainMenu = true;
	}
	else if (inSetMashTimeMenu) {
		if (dispMashTime > minTime) {
			dispMashTime--;
		}
	}
	else if (inSetBoilTimeMenu) {
		if (dispBoilTime > minTime) {
			dispBoilTime--;
		}
	}
	else if (inSetHopAddTimesMenu) {
		if (dispNumOfHopAdds > minHopAdds) {
			dispNumOfHopAdds--;
		}
	}
	else if (inHopAddSubMenu) {
		if (dispHopAddTime > minTime) {
			dispHopAddTime--;
		}
	}
	updateLcd();
	return;
}

void handleLeftBtnClick() {
	if (inStartScreen) {
		inStartScreen = false;
		inMainMenu = true;
	}
	else if (inMainMenu) {
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
		dispNumOfHopAdds = setNumOfHopAdds;
		inMainMenu = true;
	}
	else if (inHopAddSubMenu) {
		inHopAddSubMenu = false;
		for (int i = 0; i < numberOfHopAddTimes; i++) {
			hopAddTimes[i] = 0;
		}
		curNumOfHopAddTimes = 0;
		inSetHopAddTimesMenu = true;
	}
	else if (inTimerStartMenu) {
		inTimerStartMenu = false;
		inMainMenu = true;
	}
	else if (inEmptyTimersMenu) {
		inEmptyTimersMenu = false;
		mainMenuPage = 0;
		inMainMenu = true;
	}
	updateLcd();
	return;
}

void handleRightBtnClick() {
	if (inStartScreen) {
		inStartScreen = false;
		inMainMenu = true;
	}
	else if (inMainMenu) {
		mainMenuPage = (mainMenuPage + 1) % numOfMainMenuPages;
	}
	else if (inSetHopAddTimesMenu) {
		if (setNumOfHopAdds > 0) {
			numberOfHopAddTimes = setNumOfHopAdds;
			inSetHopAddTimesMenu = false;
			inHopAddSubMenu = true;
		}
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
		if (!((inSetMashTimeMenu || inSetBoilTimeMenu || inSetHopAddTimesMenu
			|| inHopAddSubMenu) && (i == 2 || i == 3))) {
			prevBtnStates[i] = btnStates[i];
		}
	}
	return pressedBtn;
}

void timerEnd() {
	for (int i = 0; i < 6; i++) {
		if (boilTimerEnded)
			digitalWrite(ledPins[1], HIGH);
		else if (mashTimerEnded) {
			digitalWrite(ledPins[0], HIGH);
			digitalWrite(ledPins[1], HIGH);
		}
		else {
			digitalWrite(ledPins[i % numberOfLeds], HIGH);
		}
		tone(speakerPin, 880, 500);
		delay(500);
		if (boilTimerEnded)
			digitalWrite(ledPins[1], LOW);
		else if (mashTimerEnded) {
			digitalWrite(ledPins[0], LOW);
			digitalWrite(ledPins[1], LOW);
		}
		else {
			digitalWrite(ledPins[i % numberOfLeds], LOW);
		}
		delay(500);
	}
}

void hopAlert(byte index) {
	lcd.clear();
	lcd.home();
	lcd.print("Hop addition:");
	lcd.setCursor(0, 1);
	lcd.print(hopAddTimes[index]);
	lcd.print(" minutes");
	for (int i = 0; i < 3; i++) {
		digitalWrite(ledPins[2], HIGH);
		tone(speakerPin, 1000, 500);
		delay(500);
		digitalWrite(ledPins[2], LOW);
		delay(500);
		if ((millis() - prevMillis > refreshInterval) && boilTimeCounter > 0) {
			prevMillis = millis();
			boilTimeCounter--;
		}
	}
	hopAddTimes[index] = -1;
	updateLcd();
	return;
}
