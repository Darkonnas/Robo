/*
 Name:		Tema3.ino
 Created:	30-Oct-19 3:49:32 PM
 Author:	vlad_
*/

const uint8_t redPotPin = A5, greenPotPin = A4, bluePotPin = A3, redPin = 9, greenPin = 10, bluePin = 11;

// the setup function runs once when you press reset or power the board
void setup() {
	//Serial.begin(9600);
	pinMode(redPotPin, INPUT);
	pinMode(greenPotPin, INPUT);
	pinMode(bluePotPin, INPUT);
	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	analogWrite(redPin, map(analogRead(redPotPin), 0, 1023, 0, 255));
	analogWrite(greenPin, map(analogRead(greenPotPin), 0, 1023, 0, 255));
	analogWrite(bluePin, map(analogRead(bluePotPin), 0, 1023, 0, 255));
	//Serial.println("R:" + String(map(redValue, 0, 1023, 0, 255)) + " G:" + String(map(greenValue, 0, 1023, 0, 255)) + " B:" + String(map(blueValue, 0, 1023, 0, 255)));
}
