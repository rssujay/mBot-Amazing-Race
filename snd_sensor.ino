#define SOUND_LW A1 // A0 && A1 are the pins associated with the microphone
#define SOUND_HI A0 // if not A0 and A1, just use a different pin

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_LW, INPUT);
  pinMode(SOUND_HI, INPUT);
}

void loop() {
  soundSense();
}

/**
 * SOUND CHALLENGE SOLVER
 */
void soundSense() {
  int fL = analogRead(SOUND_LW);
  int fH = analogRead(SOUND_HI);
  Serial.print("fL: ");
  Serial.println(fL);
  Serial.print("fH: ");
  Serial.println(fH);
  delay(1000);

  float r = (float) fL / ((float) fH + 0.01);
  Serial.println(r);
  if (r > 100)             // {300Hz > 3kHz)
    Serial.println("/-- FL IS LOUDER --/");
  else if (r < 0.5)             // {3kHz > 300Hz}
    Serial.println("/-- FH IS LOUDER --/");
  else if (r > 0.5 && r < 100) // {3kHz same as 300Hz}
    Serial.println("/-- SAME LOUDNESS --/");
}
