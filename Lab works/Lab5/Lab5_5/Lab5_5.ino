int incomingByte = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println("I received: 0x" + String(incomingByte, HEX));
  }
}
