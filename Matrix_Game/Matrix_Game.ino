#include "GameMenu.h"
#include "Game.h"

void setup() {
  
  randomSeed(analogRead(0));
  //clearEEPROM();
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT);
  for (int8_t i = 0; i < 4; ++i)
    pinMode(buttonPins[i], INPUT_PULLUP);
  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 2);
  ledControl.clearDisplay(0);
  pinMode(Joybutton_swPin, INPUT_PULLUP);
  LCD.begin(LCD_width, LCD_height);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print(Menu_topRow);
  LCD.setCursor(0, 1);
  LCD.print(Menu_bottomRow);
  EEPROM.get(0, scores);
 // Serial.begin(9600);
}

void clearEEPROM() {
  for(int i = 0; i < EEPROM.length(); ++i)
    EEPROM.put(i, 0);
}

// the loop function runs over and over again until power down or reset
void loop() {
  if (gameRunning) {
    playGame();
  } else {
    displayMenu();
  }
}
