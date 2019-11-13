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

int8_t index = 0;
bool dpState = true;

void setup() {
  // put your setup code here, to run once:
  for(int i=0; i < 8; ++i) {
    pinMode(segmentPin[i], OUTPUT);
  }
}

void displayDigit(int8_t digit, int8_t decimalPoint) {
  for(int i = 0 ; i < segmentCount - 1; ++i)
    digitalWrite(segmentPin[i], digits[digit][i]);
   digitalWrite(segmentPin[segmentCount - 1], decimalPoint);
}

void loop() {
  // put your main code here, to run repeatedly:
  displayDigit(index, dpState);
  index++;
  delay(500);
  if(index == 10) {
    index = 0;
    dpState = !dpState;
  }
}
