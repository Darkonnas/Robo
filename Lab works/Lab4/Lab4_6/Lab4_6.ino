const int photoCellPin = A0;
const int ledPin = 13;

int photoCellValue = 0;

int treshold = 30;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  photoCellValue = analogRead(photoCellPin);
  if(photoCellValue <= treshold)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);
}
