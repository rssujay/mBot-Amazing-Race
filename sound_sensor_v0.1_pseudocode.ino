// pseudocode
void setup() {
  pinMode(pin1, input)
  pinMode(pin2, input)
}

void loop() {
  fa = analogRead(pin1) // (fa is 300hz)
  fb = analogRead(pin2) // (fb is 3000hz)

  if abs(fa - fb) > threshold // threshold is variance of loudness between fa and fb
    // same loudness
    left turn
  else if abs(fb - fa) > 100
    // fb is louder than fa
    right turn  
  else if abs(fa - fb) < 100
    // fa is louder than fb
    uturn
}
