const int pushButtonPin = 2;
const int ledPin = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pushButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(pushButtonPin);
  Serial.println(buttonState);
  digitalWrite(ledPin, !buttonState);
}
