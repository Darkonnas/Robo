///7 segment display

const int8_t segmentAPin = 2;
const int8_t segmentBPin = 3;
const int8_t segmentCPin = 4;
const int8_t segmentDPin = 5;
const int8_t segmentEPin = 6;
const int8_t segmentFPin = 7;
const int8_t segmentGPin = 8;
const int8_t segmentDPPin = 9;

const int8_t segmentCount = 8;
int8_t segmentPin[] = {segmentAPin, segmentBPin, segmentCPin, segmentDPin, segmentEPin, segmentFPin,segmentGPin, segmentDPPin};

int8_t digits[][segmentCount - 1] {
  {true, true, true, true, true, true, false}, //0
  {false, true, true, false, false, false, false}, //1
  {true, true, false, true, true, false, true}, //2
  {true, true, true, true, false, false, true}, //3
  {false, true, true, false, false, true, true}, //4
  {true, false, true, true, false, true, true}, //5
  {true, false, true, true, true, true, true}, //6
  {true, true, true, false, false, false, false}, //7
  {true, true, true, true, true, true, true}, //8
  {true, true, true, true, false, true, true} //9
};

bool dpState = false;
int digit = 5;

void displayDigit(int8_t digit, int8_t decimalPoint) {
  for(int i = 0 ; i < segmentCount - 1; ++i)
    digitalWrite(segmentPin[i], digits[digit][i]);
   digitalWrite(segmentPin[segmentCount - 1], decimalPoint);
}

///Joystick

const int pushButton = 10;
const int pinX = A0;
const int pinY = A1;

int buttonValue = 0;
int xValue = 0;
int yValue = 0;

bool buttonState = false;
bool lastButtonState = false;
bool joyMoved = false;
int minTreshold = 400;
int maxTreshold = 600;

void setup() {
  for(int i=0; i < 8; ++i) {
    pinMode(segmentPin[i], OUTPUT);
  }
  pinMode(pushButton, INPUT_PULLUP);
  displayDigit(digit, dpState);
}

void loop() {
  xValue = analogRead(pinX);
  if(xValue < minTreshold && joyMoved == false) {
    if(digit > 0)
      --digit;
    else
      digit = 9;
    joyMoved = true;
  }
  if(xValue > maxTreshold && joyMoved == false) {
    if(digit < 9)
      ++digit;
    else
      digit = 0;
    joyMoved = true;
  }
  if(xValue >= minTreshold && xValue <= maxTreshold)
    joyMoved = false;
  buttonState = digitalRead(pushButton);
  if(buttonState != lastButtonState) {
    if(buttonState == true)
      buttonState = !buttonState;
    lastButtonState = buttonState;
  }
  displayDigit(digit, buttonState);
  delay(1);
}
