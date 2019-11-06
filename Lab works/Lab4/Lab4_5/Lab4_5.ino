const int buttonPin = 2;
const int ledPin = 13;

int ledState = HIGH;
int buttonState = LOW;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = digitalRead(buttonPin);
  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > debounceDelay) {
    if(reading != buttonState) {
      buttonState = reading;
      if(buttonState == LOW) {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(ledPin, ledState);
  lastButtonState = reading;
}
