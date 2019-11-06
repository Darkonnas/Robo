const int buzzerPin = 11;
const int photoCellPin = A0;

int photoCellValue = 0;

int minPitch = 120;
int maxPitch = 1500;

int minPhotoCell = 10;
int maxPhotoCell = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  photoCellValue = analogRead(photoCellPin);
  Serial.println(photoCellValue);
  int buzzerTone = map(photoCellValue, minPhotoCell, maxPhotoCell, minPitch, maxPitch);
  tone(buzzerPin, buzzerTone, 100);
  delay(100);
}
