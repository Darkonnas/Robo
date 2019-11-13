const int pushButton = 10;
const int pinX = A0;
const int pinY = A1;

int buttonValue = 0;
int xValue = 0;
int yValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pushButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonValue = !digitalRead(pushButton);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  Serial.print("Switch: ");
  Serial.print(buttonValue);
  Serial.print(" | ");
  Serial.print(" X Axis: ");
  Serial.print(xValue);
  Serial.print(" | ");
  Serial.print(" Y Axis: ");
  Serial.println(yValue);
  delay(200);
}
