#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const byte rightBtnPin = 12;
const byte upBtnPin = 11;
const byte downBtnPin = 10;
const byte leftBtnPin = 9;
const byte chooseBtnPin = 8;
int rBtnState, lBtnState, uBtnState, dBtnState, chBtnState = 0;
int prevRBtnState, prevLBtnState, prevUBtnState, prevDBtnState, prevChBtnState = 0;


void setup() {
  lcd.begin(16, 2);
  lcd.home();
  lcd.blink();
  for (int i = leftBtnPin; i < rightBtnPin+1; i++) {
    pinMode(i, INPUT);
  }
  pinMode(chooseBtnPin, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  rBtnState = digitalRead(rightBtnPin);
  lBtnState = digitalRead(leftBtnPin);
  uBtnState = digitalRead(upBtnPin);
  dBtnState = digitalRead(downBtnPin);
  chBtnState = digitalRead(chooseBtnPin);

  if (rBtnState == HIGH && rBtnState != prevRBtnState) {
    Serial.println("Right");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("White is alright");
  }
  if (lBtnState == HIGH && lBtnState != prevLBtnState) {
    Serial.println("Left");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Left");
  }
  if (uBtnState == HIGH && uBtnState != prevUBtnState) {
    Serial.println("Up");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Up");
  }
  if (dBtnState == HIGH && dBtnState != prevDBtnState) {
    Serial.println("Down");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Down");
  }
  if (chBtnState == HIGH && chBtnState != prevChBtnState) {
    Serial.println("Choose");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Confirm");
  }

  prevRBtnState = rBtnState;
  prevLBtnState = lBtnState;
  prevUBtnState = uBtnState;
  prevDBtnState = dBtnState;
  prevChBtnState = chBtnState;
  
  delay(100);
}
