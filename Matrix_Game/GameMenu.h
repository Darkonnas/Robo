#ifndef GAME_MENU_H_INCLUDED
#define GAME_MENU_H_INCLUDED

#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "Tile.h"

//Joybutton

const uint8_t Joybutton_xPin = A1;
uint16_t Joybutton_xValue;
const uint16_t Joybutton_xMinTreshold = 400;
const uint16_t Joybutton_xMaxTreshold = 600;
const uint8_t Joybutton_yPin = A0;
uint16_t Joybutton_yValue;
const uint16_t Joybutton_yMinTreshold = 400;
const uint16_t Joybutton_yMaxTreshold = 600;
const uint8_t Joybutton_swPin = 8;
bool Joybutton_swValue;
bool Joybutton_swPushed = false;
const int16_t Joybutton_Delay = 10;
uint32_t Joybutton_lastMove;
//LCD
const uint8_t LCD_width = 16;
const uint8_t LCD_height = 2;
const uint8_t LCD_RSPin = 7;
const uint8_t LCD_EnablePin = 6;
const uint8_t LCD_D4Pin = 5;
const uint8_t LCD_D5Pin = 4;
const uint8_t LCD_D6Pin = 3;
const uint8_t LCD_D7Pin = 2;

LiquidCrystal LCD(LCD_RSPin, LCD_EnablePin, LCD_D4Pin, LCD_D5Pin, LCD_D6Pin, LCD_D7Pin);

enum class Screens {
  MENU, START, SETUP, HIGHSCORES, END
};

Screens currentScreen = Screens::MENU;
bool screenChanged = false;

enum class Choices {
  MENU_START, MENU_SETUP, MENU_HIGHSCORES
};

Choices currentChoice = Choices::MENU_START, menu_YAux = Choices::MENU_START;
bool choiceChanged = false;


//Menu screen

String Menu_TopRow = ">Play     Setup ";
String Menu_BottomRow = "   HighScores   ";

//Start screen

String Start_TopRow = " Song: ";
String Start_BottomRow = "   Score: ";

//Setup screen

String Setup_TopRow = " Name: ";
String Setup_BottomRow = " Song: ";
uint8_t setupStep;
uint8_t pos = 0;
const uint8_t name_maxLength = LCD_width - Setup_TopRow.length();
String chosenName = "";
uint8_t chosenSongID;
uint8_t maxSongID = songNo;
const uint16_t pos_BlinkInterval = 375;
uint32_t pos_lastBlink = 0;
bool pos_state = true;
uint8_t chosenSong;

bool completedSetup = false;

//HighScore screen

String HighScore_TopRow;
int8_t tab, line;

struct HighScore {
  char Player[10];
  uint8_t Score;
};

HighScore scores[sizeof(Songs) / sizeof(Song)][10];

//End screen

String End_TopRow1 = "Congratulations!";
String End_BottomRow1 = "Final score: ";
const int16_t End_Timeout = 5000;
int32_t End_lastTime;
String End_TopRow2 = "Press the Button";
String End_BottomRow2 = "  to continue.  ";
const int16_t End_blinkDelay = 1000;
int32_t End_lastBlink;
bool End_blinkState;

extern int8_t countdown;
extern bool gameRunning;
extern String currentPlayer;
extern uint8_t currentScore;
extern Song currentSong;

void displayMenu() {
  if (millis() - Joybutton_lastMove >= Joybutton_Delay) {
    Joybutton_lastMove = millis();
    Joybutton_xValue = analogRead(Joybutton_xPin);
    Joybutton_yValue = analogRead(Joybutton_yPin);
    Joybutton_swValue = !digitalRead(Joybutton_swPin);
  }

  if (Joybutton_swValue == false && Joybutton_swPushed == true) {
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
                currentScreen = Screens::START;
                currentPlayer = chosenName;
                currentSong = Songs[chosenSongID];
                currentScore = 0;
                countdown = 5;
              }
              break;
            case Choices::MENU_SETUP:
              currentScreen = Screens::SETUP;
              pos = 0;
              LCD.clear();
              LCD.setCursor(0, 0);
              LCD.print(Setup_TopRow);
              LCD.setCursor(0, 1);
              LCD.print(Setup_BottomRow + chosenSongID);
              break;
            case Choices::MENU_HIGHSCORES:
              currentScreen = Screens::HIGHSCORES;
              tab = songNo;
              for (int i = 0; i < songNo; ++i) {
                if (scores[i][0].Score > 0) {
                  tab = i;
                  break;
                }
              }
              if (tab != songNo) {
                line = 0;
                LCD.clear();
                LCD.setCursor(0, 0);
                LCD.print(Songs[tab].Name());
                LCD.setCursor(0, 1);
                LCD.print(String(line + 1) + ". " + scores[tab][line].Player + ' ' + String(scores[tab][line].Score));
              } else {
                LCD.clear();
                LCD.setCursor(0, 0);
                LCD.print("  No entries!   ");
              }
              break;
            default:
              break;
          }
        }
      }
      else {
        if (choiceChanged == false && Joybutton_xValue < Joybutton_xMinTreshold) {
          switch (currentChoice) {
            case Choices::MENU_SETUP:
              currentChoice = Choices::MENU_START;
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
            default:
              break;
          }
        }
        if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
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
        }
        if (choiceChanged == true && Joybutton_xValue >= Joybutton_xMinTreshold && Joybutton_xValue <= Joybutton_xMaxTreshold/* && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold*/) {
          choiceChanged = false;
          switch (currentChoice) {
            case Choices::MENU_START:
              Menu_TopRow = ">Play     Setup ";
              Menu_BottomRow = "   HighScores  ";
              break;
            case Choices::MENU_SETUP:
              Menu_TopRow = " Play    >Setup ";
              Menu_BottomRow = "   HighScores   ";
              break;
            case Choices::MENU_HIGHSCORES:
              Menu_TopRow = " Play     Setup ";
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
      LCD.clear();
      LCD.setCursor(0, 0);
      Serial.println(Start_TopRow + String(currentSong.Name()));
      LCD.print(Start_TopRow + currentSong.Name());
      LCD.setCursor(0, 1);
      LCD.print(Start_BottomRow + String(currentScore));
      gameRunning = true;
      break;
    case Screens::SETUP:
      switch (setupStep) {
        case 0:
          if (choiceChanged == false && Joybutton_swValue == true) {
            //Serial.println(String(Joybutton_swPushed) + ' ' + String(Joybutton_swValue));
            if (Joybutton_swPushed == false) {
              while (chosenName[chosenName.length() - 1] == ' ')
                chosenName = chosenName.substring(0, chosenName.length() - 1);
              if (chosenName.length() == 0)
                chosenName = "UNKNOWN";
              Joybutton_swPushed = true;
              setupStep = 1;
              pos_state = true;
              pos_lastBlink = millis();
            }
          }
          else {
            //Serial.println(Joybutton_swPushed);
            if (chosenName.length() > 0 && choiceChanged == false && Joybutton_xValue < Joybutton_xMinTreshold) {
              if (pos == chosenName.length() - 1 && chosenName[pos] == ' ')
                chosenName = chosenName.substring(0, chosenName.length() - 1);
              if (pos == 0)
                pos = chosenName.length() - 1;
              else
                --pos;
              pos_lastBlink = millis();
              pos_state = true;
              choiceChanged = true;
            }
            if (choiceChanged == false && Joybutton_xValue > Joybutton_xMaxTreshold) {
              if (pos >= chosenName.length())
                for (int i = chosenName.length(); i <= pos; ++i)
                  chosenName.concat(" ");
              if (pos == name_maxLength - 1) {
                while (chosenName[chosenName.length() - 1] == ' ')
                  chosenName = chosenName.substring(0, chosenName.length() - 1);
                pos = 0;
              }
              else
                ++pos;
              pos_lastBlink = millis();
              pos_state = true;
              choiceChanged = true;
            }
            if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
              if (pos == chosenName.length())
                chosenName.concat(" ");
              if (chosenName[pos] == 'A')
                chosenName[pos] = ' ';
              else if (chosenName[pos] == ' ')
                chosenName[pos] = 'Z';
              else
                --chosenName[pos];
              choiceChanged = true;
            }
            if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
              if (pos == chosenName.length())
                chosenName.concat(" ");
              if (chosenName[pos] == 'Z')
                chosenName[pos] = ' ';
              else if (chosenName[pos] == ' ')
                chosenName[pos] = 'A';
              else
                ++chosenName[pos];
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
                LCD.print(chosenName);
              else {
                LCD.print(chosenName.substring(0, pos));
                LCD.print('_');
                LCD.print(chosenName.substring(pos + 1, chosenName.length()));
              }
              LCD.setCursor(0, 1);
              LCD.print(Setup_BottomRow + Songs[chosenSongID].Name());
            }
          }
          break;
        case 1:
          if (choiceChanged == false && Joybutton_swValue == true) {
            if (Joybutton_swPushed == false) {
              Joybutton_swPushed = true;
              setupStep = 0;
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
              if (chosenSongID == 0)
                chosenSongID = maxSongID - 1;
              else
                --chosenSongID;
              choiceChanged = true;
            }
            if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
              if (chosenSongID == maxSongID - 1)
                chosenSongID = 0;
              else
                ++chosenSongID;
              choiceChanged = true;
            }
            if (choiceChanged == true && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold)
              choiceChanged = false;
            if (millis() - pos_lastBlink >= pos_BlinkInterval) {
              LCD.clear();
              pos_lastBlink = millis();
              pos_state = !pos_state;
              LCD.setCursor(0, 0);
              LCD.print(Setup_TopRow + chosenName);
              LCD.setCursor(0, 1);
              LCD.print(Setup_BottomRow);
              if (pos_state == true)
                LCD.print(Songs[chosenSongID].Name());
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
        if (choiceChanged == false && Joybutton_xValue < Joybutton_xMinTreshold) {
          if (tab > 0)
            --tab;
          choiceChanged = true;
        }
        if (choiceChanged == false && Joybutton_xValue > Joybutton_xMaxTreshold) {
          if (tab < songNo - 1 && scores[tab + 1][0].Score > 0)
            ++tab;
          choiceChanged = true;
        }
        if (choiceChanged == false && Joybutton_yValue > Joybutton_yMaxTreshold) {
          if (line < 9 && scores[tab][line + 1].Score > 0)
            ++line;
          choiceChanged = true;
        }
        if (choiceChanged == false && Joybutton_yValue < Joybutton_yMinTreshold) {
          if (line > 0)
            --line;
          choiceChanged = true;
        }
        if (choiceChanged == true && Joybutton_xValue >= Joybutton_xMinTreshold && Joybutton_xValue <= Joybutton_xMaxTreshold && Joybutton_yValue >= Joybutton_yMinTreshold && Joybutton_yValue <= Joybutton_yMaxTreshold) {
          choiceChanged = false;
          LCD.clear();
          LCD.setCursor(0, 0);
          LCD.print(Songs[tab].Name());
          LCD.setCursor(0, 1);
          LCD.print(String(line + 1) + ". " + scores[tab][line].Player + ' ' + String(scores[tab][line].Score));
        }
      }
      break;
    case Screens::END:
      if (millis() - End_lastTime >= End_Timeout) {
        if (Joybutton_swValue && !Joybutton_swPushed) {
          Joybutton_swPushed = true;
          currentScreen = Screens::MENU;
          LCD.clear();
          LCD.setCursor(0, 0);
          LCD.print(Menu_TopRow);
          LCD.setCursor(0, 1);
          LCD.print(Menu_BottomRow);
        }
        if (millis() - End_lastBlink >= End_blinkDelay) {
          End_lastBlink = millis();
          End_blinkState = !End_blinkState;
          if (End_blinkState) {
            LCD.setCursor(0, 0);
            LCD.print(End_TopRow2);
            LCD.setCursor(0, 1);
            LCD.print(End_BottomRow2);
          } else {
            LCD.clear();
          }
        }
      }
      break;
    default:
      break;
  }
}

#endif
