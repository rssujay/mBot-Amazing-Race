#include <math.h>
#define SOUND_LW A0 // A0 && A1 are the pins associated with the microphone
#define SOUND_HI A1 // if not A0 and A1, just use a different pin
#define 

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

  int r = fL / fH;
  if (r > 1.25)
    // {300Hz > 3kHz)
    Serial.println("/-- FL IS LOUDER --/");
  if (r < 0.75)
    // {3kHz > 300Hz}
    Serial.println("/-- FH IS LOUDER --/");
  if (r > 0.75 || r < 1.25)
    // {3kHz same as 300Hz}
    Serial.println("/-- SAME LOUDNESS --/");
}
