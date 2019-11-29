/*
 Name:		Tema6.ino
 Created:	28-Nov-19 9:19:52 PM
 Author:	vlad_
*/

#include <LiquidCrystal.h>
#include <EEPROM.h>

//Joybutton

const uint8_t Joybutton_xPin = A0;
const uint16_t Joybutton_xMinTreshold = 400;
const uint16_t Joybutton_xMaxTreshold = 600;
const uint8_t Joybutton_yPin = A1;
const uint16_t Joybutton_yMinTreshold = 400;
const uint16_t Joybutton_yMaxTreshold = 600;
const uint8_t Joybutton_swPin = 7;
bool Joybutton_swPushed = false;

//LCD
const uint8_t LCD_width = 16;
const uint8_t LCD_height = 2;
const uint8_t LCD_RSPin = 12;
const uint8_t LCD_EnablePin = 11;
const uint8_t LCD_D4Pin = 5;
const uint8_t LCD_D5Pin = 4;
const uint8_t LCD_D6Pin = 3;
const uint8_t LCD_D7Pin = 2;

LiquidCrystal LCD(LCD_RSPin, LCD_EnablePin, LCD_D4Pin, LCD_D5Pin, LCD_D6Pin, LCD_D7Pin);

enum class Screens {
	MENU, START, SETUP, HIGHSCORES
};

Screens currentScreen = Screens::MENU;
bool screenChanged = false;

enum class Choices {
	MENU_START, MENU_SETUP, MENU_HIGHSCORES
};

Choices currentChoice = Choices::MENU_START, menu_YAux = Choices::MENU_START;
bool choiceChanged = false;


//Menu screen

String Menu_TopRow = ">Start    Setup ";
String Menu_BottomRow = "   HighScores   ";

//Start screen

String Start_TopRow_FirstHalf = "Lives:";
String Start_TopRow_SecondHalf = "Level:";
String Start_BottomRow = "   Score: ";

String current_player;
const uint8_t start_lives = 3;
uint8_t current_lives;
uint8_t current_level;
uint8_t current_score;
const uint8_t score_multiplier = 3;
uint32_t game_start;
uint32_t level_start;
uint32_t level_time = 5000;
uint32_t game_time = 10000;
bool game_ended = false;

//Setup screen

String Setup_TopRow = " Name: ";
String Setup_BottomRow = " Level: ";
bool step = 0;
uint8_t pos = 0;
const uint8_t name_maxLength = LCD_width - Setup_TopRow.length();
String chosen_name = "";
const uint8_t pos_BlinkInterval = 500;
uint32_t pos_lastBlink = 0;
bool pos_state = true;
uint8_t chosen_level = 0;
const uint8_t max_level = 9;

bool completedSetup = false;

//HighScore screen

struct HighScore {
	char Player[10];
	uint8_t Score;
};

const uint8_t maximum_count = 10;
uint8_t count;
HighScore scores[maximum_count] = { {"XXXXXXXXX", 0} };
uint8_t offset;

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(Joybutton_swPin, INPUT_PULLUP);
	LCD.begin(LCD_width, LCD_height);
	LCD.clear();
	LCD.setCursor(0, 0);
	LCD.print(Menu_TopRow);
	LCD.setCursor(0, 1);
	LCD.print(Menu_BottomRow);
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	int Joybutton_xValue = analogRead(Joybutton_xPin);
	int Joybutton_yValue = analogRead(Joybutton_yPin);
	bool Joybutton_swValue = !digitalRead(Joybutton_swPin);
	
	if (Joybutton_swValue == false && Joybutton_swPushed == true) {
		//Serial.println("Reset");
		Joybutton_swPushed = false;
	}

	switch (currentScreen) {
	case Screens::MENU:
		if (choiceChanged == false && Joybutton_swValue == true) {
			if (Joybutton_swPushed == false) {
				Joybutton_swPushed = true;
				switch (currentChoice) {
				case Choices::MENU_START:
					if (completedSetup == true) {
						game_ended = false;
						currentScreen = Screens::START;
						current_player = chosen_name;
						game_start = level_start = millis();
						current_lives = start_lives;
						current_level = chosen_level;
						current_score = current_level * score_multiplier;
						LCD.clear();
						LCD.setCursor(0, 0);
						LCD.print(Start_TopRow_FirstHalf + String(current_lives) + ' ' + Start_TopRow_SecondHalf + String(current_level));
						LCD.setCursor(0, 1);
						LCD.print(Start_BottomRow + String(current_score));
					}
					break;
				case Choices::MENU_SETUP:
					currentScreen = Screens::SETUP;
					LCD.clear();
					LCD.setCursor(0, 0);
					LCD.print(Setup_TopRow);
					LCD.setCursor(0, 1);
					LCD.print(Setup_BottomRow + chosen_level);
					break;
				case Choices::MENU_HIGHSCORES:
					currentScreen = Screens::HIGHSCORES;
					LCD.clear();
					EEPROM.get(0, count);
					Serial.println(count);
					LCD.setCursor(0, 0);
					if (count == 0) {
						LCD.print("1. " + String(scores[0].Player));
						for (int i = 0; i < LCD_width - 5 - strlen(scores[0].Player); ++i)
							LCD.print(' ');
						LCD.print(String(scores[0].Score));
					}
					else {
						EEPROM.get(1, scores);
						LCD.print("1. " + String(scores[0].Player));
						for (int i = 0; i < LCD_width - 5 - strlen(scores[0].Player); ++i)
							LCD.print(' ');
						LCD.print(String(scores[0].Score));
						if (count > 1) {
							LCD.setCursor(0, 1);
							LCD.print("1. " + String(scores[1].Player));
							for (int i = 0; i < LCD_width - 5 - strlen(scores[1].Player); ++i)
								LCD.print(' ');
							LCD.print(String(scores[1].Score));
						}
					}
					offset = 0;
					break;
				default:
					break;
				}	
			}
		}
		else {
			if (choiceChanged == false && Joybutton_xValue < Joybutton_xMinTreshold) {
				switch (currentChoice) {
				case Choices::MENU_START:
					currentChoice = Choices::MENU_HIGHSCORES;
					choiceChanged = true;
					break;
				case Choices::MENU_SETUP:
					currentChoice = Choices::MENU_START;
					choiceChanged = true;
					break;
				case Choices::MENU_HIGHSCORES:
					currentChoice = Choices::MENU_SETUP;
					choiceChanged = true;
					break;
				default:
					break;
				}
			}
			if (choiceChanged == false && Joybutton_xValue > Joybutton_xMaxTreshold) {
				switch (currentChoice) {
				case Choices::MENU_START:
					currentChoice = Choices::MENU_SETUP;
					choiceChanged = true;
					break;
				case Choices::MENU_SETUP:
					currentChoice = Choices::MENU_HIGHSCORES;
					choiceChanged = true;
					break;
				case Choices::MENU_HIGHSCORES:
					currentChoice = Choices::MENU_START;
					choiceChanged = true;
					break;
				default:
					break;
				}
			}
			/*if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
				switch (currentChoice) {
				case Choices::MENU_START:
					menu_YAux = Choices::MENU_START;
					currentChoice = Choices::MENU_HIGHSCORES;
					choiceChanged = true;
					break;
				case Choices::MENU_SETUP:
					menu_YAux = Choices::MENU_SETUP;
					currentChoice = Choices::MENU_HIGHSCORES;
					choiceChanged = true;
					break;
				default:
					break;
				}
			}
			if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
				switch (currentChoice) {
				case Choices::MENU_HIGHSCORES:
					currentChoice = menu_YAux;
					choiceChanged = true;
					break;
				default:
					break;
				}
			} */
			if (choiceChanged == true && Joybutton_xValue >= Joybutton_xMinTreshold && Joybutton_xValue <= Joybutton_xMaxTreshold/* && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold*/) {
				choiceChanged = false;
				switch (currentChoice) {
				case Choices::MENU_START:
					Menu_TopRow = ">Start    Setup ";
					Menu_BottomRow = "   HighScores  ";
					break;
				case Choices::MENU_SETUP:
					Menu_TopRow = " Start   >Setup ";
					Menu_BottomRow = "   HighScores   ";
					break;
				case Choices::MENU_HIGHSCORES:
					Menu_TopRow = " Start    Setup ";
					Menu_BottomRow = "  >HighScores   ";
					break;
				default:
					break;
				}
				LCD.clear();
				LCD.setCursor(0, 0);
				LCD.print(Menu_TopRow);
				LCD.setCursor(0, 1);
				LCD.print(Menu_BottomRow);
			}
		}
		break;
	case Screens::START:
		if (game_ended == true) {
			if (Joybutton_swValue == true && Joybutton_swPushed == false) {
				Joybutton_swPushed = true;
				int i;
				for (i = 0; i < count; ++i) {
					if (scores[i].Score < current_score) {
						if (count < maximum_count)
							++count;
						for (int j = count - 1; j > i; --j)
							scores[j] = scores[j - 1];
						strcpy(scores[i].Player, current_player.c_str());
						scores[i].Score = current_score;
						break;
					}
				}
				if (i == count && i < maximum_count) {
					strcpy(scores[count].Player, current_player.c_str());
					scores[count].Score = current_score;
					++count;
				}
				EEPROM.put(0, count);
				EEPROM.put(1, scores);
				currentScreen = Screens::MENU;
				LCD.clear();
				LCD.setCursor(0, 0);
				LCD.print(Menu_TopRow);
				LCD.setCursor(0, 1);
				LCD.print(Menu_BottomRow);
			}
		}
		else {
			if (millis() - level_start >= level_time) {
				level_start = millis();
				++current_level;
				current_score += score_multiplier;
				LCD.clear();
				LCD.setCursor(0, 0);
				LCD.print(Start_TopRow_FirstHalf + String(current_lives) + ' ' + Start_TopRow_SecondHalf + String(current_level));
				LCD.setCursor(0, 1);
				LCD.print(Start_BottomRow + String(current_score));
			}
			if (millis() - game_start >= game_time) {
				game_ended = true;
				LCD.clear();
				LCD.setCursor(0, 0);
				LCD.print("Congratulations!");
				LCD.setCursor(0, 1);
				LCD.print("Press SW to exit");
			}
		}
		break;
	case Screens::SETUP:
		switch (step) {
		case 0:
			if (choiceChanged == false && Joybutton_swValue == true) {
				//Serial.println(String(Joybutton_swPushed) + ' ' + String(Joybutton_swValue));
				if (Joybutton_swPushed == false) {
					while (chosen_name[chosen_name.length() - 1] == ' ')
						chosen_name = chosen_name.substring(0, chosen_name.length() - 1);
					if (chosen_name.length() == 0)
						chosen_name = "UNKNOWN";
					Joybutton_swPushed = true;
					step = !step;
					pos_state = true;
					pos_lastBlink = millis();
				}
			}
			else {
				//Serial.println(Joybutton_swPushed);
				if (chosen_name.length() > 0 && choiceChanged == false && Joybutton_xValue < Joybutton_xMinTreshold) {
					if(pos == chosen_name.length() - 1 && chosen_name[pos] == ' ')
						chosen_name = chosen_name.substring(0, chosen_name.length() - 1);
					if (pos == 0)
						pos = chosen_name.length() - 1;
					else
						--pos;
					pos_lastBlink = millis();
					pos_state = true;
					choiceChanged = true;
				}
				if (choiceChanged == false && Joybutton_xValue > Joybutton_xMaxTreshold) {
					if (pos >= chosen_name.length())
						for (int i = chosen_name.length(); i <= pos; ++i)
							chosen_name.concat(" ");
					if (pos == name_maxLength - 1) {
						while (chosen_name[chosen_name.length() - 1] == ' ')
							chosen_name = chosen_name.substring(0, chosen_name.length() - 1);
						pos = 0;
					}
					else
						++pos;
					pos_lastBlink = millis();
					pos_state = true;
					choiceChanged = true;
				}
				if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
					if (pos == chosen_name.length())
							chosen_name.concat(" ");
					if (chosen_name[pos] == 'A')
						chosen_name[pos] = ' ';
					else if (chosen_name[pos] == ' ')
						chosen_name[pos] = 'Z';
					else
						--chosen_name[pos];
					choiceChanged = true;
				}
				if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
					if (pos == chosen_name.length())
						chosen_name.concat(" ");
					if (chosen_name[pos] == 'Z')
						chosen_name[pos] = ' ';
					else if (chosen_name[pos] == ' ')
						chosen_name[pos] = 'A';
					else
						++chosen_name[pos]; 
					choiceChanged = true;
				}
				if (choiceChanged == true && Joybutton_xValue >= Joybutton_xMinTreshold && Joybutton_xValue <= Joybutton_xMaxTreshold && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold)
					choiceChanged = false;
				if (millis() - pos_lastBlink >= pos_BlinkInterval) {
					LCD.clear();
					pos_lastBlink = millis();
					pos_state = !pos_state;
					LCD.setCursor(0, 0);
					LCD.print(Setup_TopRow);
					if (pos_state == true)
						LCD.print(chosen_name);
					else {
						LCD.print(chosen_name.substring(0, pos));
						LCD.print('_');
						LCD.print(chosen_name.substring(pos + 1, chosen_name.length()));
					}
					LCD.setCursor(0, 1);
					LCD.print(Setup_BottomRow + chosen_level);
				}
			}
			break;
		case 1:
			if (choiceChanged == false && Joybutton_swValue == true) {
				if (Joybutton_swPushed == false) {
					Joybutton_swPushed = true;
					step = !step;
					completedSetup = true;
					pos_state = true;
					pos_lastBlink = millis();
					currentScreen = Screens::MENU;
					LCD.clear();
					LCD.setCursor(0, 0);
					LCD.print(Menu_TopRow);
					LCD.setCursor(0, 1);
					LCD.print(Menu_BottomRow);
				}
			}
			else {
				if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
					if (chosen_level == 0)
						chosen_level = max_level;
					else
						--chosen_level;
					choiceChanged = true;
				}
				if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
					if (chosen_level == max_level)
						chosen_level = 0;
					else
						++chosen_level;
					choiceChanged = true;
				}
				if (choiceChanged == true && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold)
					choiceChanged = false;
				if (millis() - pos_lastBlink >= pos_BlinkInterval) {
					LCD.clear();
					pos_lastBlink = millis();
					pos_state = !pos_state;
					LCD.setCursor(0, 0);
					LCD.print(Setup_TopRow + chosen_name);
					LCD.setCursor(0, 1);
					LCD.print(Setup_BottomRow);
					if (pos_state == true)
						LCD.print(chosen_level);
				}
			}
			break;
		default:
			break;
		}
		break;
	case Screens::HIGHSCORES:
		if (choiceChanged == false && Joybutton_swValue == true) {
			if (Joybutton_swPushed == false) {
				Joybutton_swPushed = true;
				currentScreen = Screens::MENU;
				LCD.clear();
				LCD.setCursor(0, 0);
				LCD.print(Menu_TopRow);
				LCD.setCursor(0, 1);
				LCD.print(Menu_BottomRow);
			}
		}
		else {
			if (count != 0) {
				if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
					if (offset < (count - 1) / 2)
						++offset;
					choiceChanged = true;
				}
				if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
					if (offset > 0)
						--offset;
					choiceChanged = true;
				}
				if (choiceChanged == true && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold) {
					choiceChanged = false;
					LCD.clear();
					LCD.setCursor(0, 0);
					LCD.print(String(2 * offset + 1) + ". " + String(scores[2 * offset].Player));
					for (int i = 0; i < LCD_width - 5 - strlen(scores[2 * offset].Player); ++i)
						LCD.print(' ');
					LCD.print(String(scores[2 * offset].Score));
					if (2 * offset + 1 < count) {
						LCD.setCursor(0, 1);
						LCD.print(String(2 * (offset + 1)) + ". " + String(scores[2 * offset + 1].Player));
						for (int i = 0; i < LCD_width - 5 - strlen(scores[2 * offset + 1].Player); ++i)
							LCD.print(' ');
						LCD.print(String(scores[2 * offset + 1].Score));
					}
				}
			}
		}
		break;
	default:
		break;
	}
}
