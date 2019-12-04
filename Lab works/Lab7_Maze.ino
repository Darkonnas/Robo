#include "LedControl.h"

///Joystick

struct Joystick {
  const uint8_t xPin = A1, yPin = A0, swPin = 7;
  int xValue, yValue, minTreshold = 400, maxTreshold = 600;
  bool moved = false;
} js;

LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No of drivers

const bool maze[8][8] = {
  {1, 1, 1, 1, 0, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 0, 1},
  {1, 0, 1, 0, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 0, 1, 1, 1}
};

struct Pos {
  uint8_t row, col;
} mazeStart = {0, 4}, mazeEnd = {7, 4};

struct Bomb {
  uint8_t row, col;
  static const int blinkDelay = 500;
  static uint64_t lastBlink;
  static bool state, hit;
} mazeBombs[4] = { {2, 3}, {4, 6}, {2, 1}, {6, 1} };

uint64_t Bomb::lastBlink = 0;
bool Bomb::state = true, Bomb::hit = false;

struct Player {
  uint8_t row = mazeStart.row, col = mazeStart.col;
  static const int blinkDelay = 100;
  static uint64_t lastBlink;
  static bool state;
} currentPlayer;

uint64_t Player::lastBlink = 0;
bool Player::state = true;

void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0, 2); // 0 15
  lc.clearDisplay(0);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((currentPlayer.row == mazeEnd.row && currentPlayer.col == mazeEnd.col) || Bomb::hit) {
    for (int row = 0; row < 8; row ++)
      for (int col = 0; col < 8; col++)
        lc.setLed(0, row, col, false);
  }
  else {
    js.xValue = analogRead(js.xPin);
    js.yValue = analogRead(js.yPin);

    if (!js.moved && js.xValue < js.minTreshold) {
      js.moved = true;
      if (currentPlayer.col != 0) {
        if (!maze[currentPlayer.row][currentPlayer.col - 1])
          --currentPlayer.col;
      }
    }

    if (!js.moved && js.xValue > js.maxTreshold) {
      js.moved = true;
      if (currentPlayer.col != 7) {
        if (!maze[currentPlayer.row][currentPlayer.col + 1])
          ++currentPlayer.col;
      }
    }

    if (!js.moved && js.yValue < js.minTreshold) {
      js.moved = true;
      if (currentPlayer.row != 0) {
        if (!maze[currentPlayer.row - 1][currentPlayer.col])
          --currentPlayer.row;
      }
    }

    if (!js.moved && js.yValue > js.maxTreshold) {
      js.moved = true;
      if (currentPlayer.row != 7) {
        if (!maze[currentPlayer.row + 1][currentPlayer.col])
          ++currentPlayer.row;
      }
    }

    if (js.moved && js.xValue >= js.minTreshold && js.xValue <= js.maxTreshold && js.yValue >= js.minTreshold && js.yValue <= js.maxTreshold)
      js.moved = false;

    if (millis() - Player::lastBlink >= Player::blinkDelay) {
      Player::lastBlink = millis();
      Player::state = !Player::state;
    }

    if (millis() - Bomb::lastBlink >= Bomb::blinkDelay) {
      Bomb::lastBlink = millis();
      Bomb::state = !Bomb::state;
    }

    for (int row = 0; row < 8; row ++) {
      for (int col = 0; col < 8; col++) {
        if (row == currentPlayer.row && col == currentPlayer.col)
          lc.setLed(0, row, col, Player::state);
        else {
          int b;
          for (b = 0; b < 4; ++b) {
            if (row == mazeBombs[b].row && col == mazeBombs[b].col) {
              lc.setLed(0, row, col, Bomb::state);
              break;
            }
          }
          if (b == 4)
            lc.setLed(0, row, col, maze[row][col]);
        }
      }
    }
    for (int b = 0; b < 4; ++b) {
      if (mazeBombs[b].row == currentPlayer.row && mazeBombs[b].col == currentPlayer.col) {
        Bomb::hit = true;
        break;
      }
    }
  }
}
