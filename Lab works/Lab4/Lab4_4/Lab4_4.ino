const int pushButtonPin = 2;
int counter = 0;
bool pushDebouncer = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(pushButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(pushButtonPin);
  buttonState = !buttonState;
  unsigned long currentDebounceTime = millis();
  if(currentDebounceTime - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentDebounceTime;
    if(buttonState == HIGH && pushDebouncer == false) {
      pushDebouncer = true;
      Serial.println(++counter);
    }
    if(buttonState == LOW && pushDebouncer == true) {
      pushDebouncer = false;
    }
  }
}
