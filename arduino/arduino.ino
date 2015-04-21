/* Det som finns för nuvarande på arduinon */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
}

bool lastValue = false;

void loop() {
  bool Value = (digitalRead(2) == HIGH) ? true : false; 
  
  if (!lastValue && Value) {
    Serial.print("wifi-scan-plz");
    Serial.write(0);
    }
  
  lastValue = Value;
  delay(5);
}
