#define magnetPin 22

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  digitalWrite(magnetPin, HIGH); // turn the Electromagnet on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  //digitalWrite(magnetPin, LOW);// turn the Electromagnet off by making the voltage LOW
  delay(1000);  
}
