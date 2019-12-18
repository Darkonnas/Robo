#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <LedControl.h>
LedControl ledControl(12, 11, 10, 1);

#include "Tile.h"

const byte digits[][8] = {
  {
    B00000000,
    B00011000,
    B00011000,
    B00111000,
    B00011000,
    B00011000,
    B00011000,
    B01111110
  }, {
    B00000000,
    B00111100,
    B01100110,
    B00000110,
    B00001100,
    B00110000,
    B01100000,
    B01111110
  }, {
    B00000000,
    B00111100,
    B01100110,
    B00000110,
    B00011100,
    B00000110,
    B01100110,
    B00111100
  }, {
    B00000000,
    B00001100,
    B00011100,
    B00101100,
    B01001100,
    B01111110,
    B00001100,
    B00001100
  }, {
    B00000000,
    B01111110,
    B01100000,
    B01111100,
    B00000110,
    B00000110,
    B01100110,
    B00111100
  }, {
    B00000000,
    B00111100,
    B01100110,
    B01100000,
    B01111100,
    B01100110,
    B01100110,
    B00111100
  }, {
    B00000000,
    B01111110,
    B01100110,
    B00001100,
    B00001100,
    B00011000,
    B00011000,
    B00011000
  }
};

int8_t countdown;
int32_t lastCount;
bool gameRunning;
String currentPlayer;
uint8_t currentScore;
Song currentSong;

const int16_t buttonDelay = 10;
uint32_t lastPress;
const int8_t buzzerPin = 9;
const int8_t buttonPins[4] = { A2, A3, A4, A5 };
bool buttonState[4];
bool needsReset[4];

int32_t lastDrop, lastSink;
double speedMult = .5;
bool tileMissed;

Tile screenTiles[4];

void dropTile(const Vector2D& Position = Vector2D(), const Vector2D& Size = Vector2D()) {
  for (int8_t i = 0; i < 3; ++i)
    screenTiles[i] = screenTiles[i + 1];
  if (currentSong.Position() < currentSong.Length()) {
    screenTiles[3] = {currentSong[currentSong.Position()++], Position, Size};
    if (screenTiles[3].Pitch() == Pitches::Null)
      screenTiles[3].Size().x() = 0;
  }
  else {
    if (screenTiles[1].Position().x() == -4) {
      speedMult *= 2;
      currentSong.Position() = 0;
    }
    else {
      screenTiles[3] = Tile();
    }
  }
}

void sinkTiles() {
  for (int8_t i = 0; i < 4; ++i) {
    int8_t& tileRow = screenTiles[i].Position().x();
    if (tileRow == -4)
      continue;
    int8_t tileCol = screenTiles[i].Position().y();
    int8_t tileColNo = screenTiles[i].Size().y();
    int8_t tileRowNo = screenTiles[i].Size().x();
    if (tileRowNo > 0) {
      if (tileRow >= 0) {
        for (int8_t col = tileCol; col < tileCol + tileColNo; ++col)
          ledControl.setLed(0, tileRow, col, false);
      }
      if (tileRow + tileRowNo >= 0 && tileRow + tileRowNo < 8) {
        for (int8_t col = tileCol; col < tileCol + tileColNo; ++col)
          ledControl.setLed(0, tileRow + tileRowNo, col, true);
      }
    }
    ++tileRow;
    if (tileRowNo > 0 && tileRow >= 8) {
      tileMissed = true;
      return;
    }
  }
}

void playZone(const int8_t& zone) {
  bool wrongOrder = false;
  int8_t tileFound = 4;
  for (int i = 0; i < 4; ++i) {
    if (screenTiles[i].Size().x() > 0 && screenTiles[i].Position().x() > -3 && screenTiles[i].Position().x() < 6) {
      if (screenTiles[i].Position().y() / 2 == zone) {
        tileFound = i;
        break;
      }
      else
        wrongOrder = true;
    }
  }
  //Serial.print(wrongOrder);
  if (tileFound != 4) {
    if (!wrongOrder) {
      noTone(buzzerPin);
      tone(buzzerPin, screenTiles[tileFound].Pitch(), currentSong.TimeUnit() / speedMult);
      for (int i = max(screenTiles[tileFound].Position().x(), 0); i < max(screenTiles[tileFound].Position().x(), 0) + screenTiles[tileFound].Size().x(); ++i)
        for (int j = screenTiles[tileFound].Position().y(); j < screenTiles[tileFound].Position().y() + screenTiles[tileFound].Size().y(); ++j)
          ledControl.setLed(0, i, j, false);
      screenTiles[tileFound].Size().x() = 0;
      ++currentScore;
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print(Start_topRow);
      LCD.print(currentSong.Name());
      LCD.setCursor(0, 1);
      LCD.print(Start_bottomRow);
      LCD.print(currentScore);
    }
  } else {
    tileMissed = true;
    return;
  }
  needsReset[zone] = true;
}

int8_t lastRandom = -1;

void playGame() {
  if (countdown >= -1 ) {
    if (millis() - lastCount >= 1000) {
      if (countdown > -1) {
        lastCount = millis();
        if (!countdown)
          ledControl.clearDisplay(0);
        else {
          for (int i = 0; i < 8; ++i)
            ledControl.setRow(0, i, digits[countdown - 1][i]);
        }
      }
      --countdown;
    }
  } else {
    if (tileMissed) {
      noTone(buzzerPin);
      ledControl.clearDisplay(0);
      gameRunning = false;
      currentScreen = Screens::END;
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print(End_topRow1);
      LCD.setCursor(0, 1);
      LCD.print(End_bottomRow1);
      LCD.print(currentScore);
      End_lastTime = millis();
      tileMissed = false;
      for (int i = 0; i < 4; ++i)
        screenTiles[i] = Tile();
      if (currentScore > 0) {
        for (int i = 0; i < 10; ++i) {
          if (scores[chosenSongID][i].Score < currentScore) {
            int j;
            for (j = 9; j > i && scores[chosenSongID][j].Score == 0; --j);
            j = min(9, j + 1);
            for (; j > i; --j) {
              strcpy(scores[chosenSongID][j].Player, scores[chosenSongID][j - 1].Player);
              scores[chosenSongID][j].Score = scores[chosenSongID][j - 1].Score;
            }
            strcpy(scores[chosenSongID][i].Player, currentPlayer.c_str());
            scores[chosenSongID][i].Score = currentScore;
            break;
          }
        }
        EEPROM.put(0, scores);
      }
    }
    else {
      const int32_t currMillis = millis();
      // put your main code here, to run repeatedly:
      if (currMillis - lastPress >= buttonDelay) {
        lastPress = currMillis;
        for (int8_t i = 0; i < 4; ++i) {
          buttonState[i] = !digitalRead(buttonPins[i]);
          if (buttonState[i] && !needsReset[i])
            playZone(i);
          else if (needsReset[i] && buttonState[i] == false)
            needsReset[i] = false;
        }
      }
      if (currMillis - lastDrop >= currentSong.TimeUnit() / speedMult) {
        lastDrop = currMillis;
        int8_t currRandom;
        do {
          currRandom = random(0, 3);
        } while (currRandom == lastRandom);
        lastRandom = currRandom;
        dropTile({ -3, (int8_t)(2 * currRandom)}, {currentSong.SizeUnit(), 2});
      }
      if (currMillis - lastSink >= currentSong.TimeUnit() / (speedMult * currentSong.SizeUnit())) {
        lastSink = currMillis;
        sinkTiles();
      }
    }
  }
}

#endif
