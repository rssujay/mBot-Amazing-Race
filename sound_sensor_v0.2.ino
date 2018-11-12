#include <math.h>
#define SND_THSHOLD 100
#define SOUND_LW A0 // A0 && A1 are the pins assoicated with the microphone
#define SOUND_HI A1 // use a different pin later

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
  int fA = analogRead(SOUND_LW);
  Serial.print("fA: ");
  Serial.println(fA);
  delay(500);

  int fB = analogRead(SOUND_HI);  
  Serial.print("fB: ");
  Serial.println(fB);
  delay(500);

  if (abs(fA - fB) < SND_THSHOLD) {
    // {fA & fB same loudness}
    Serial.println("RESULT: same loudness");
//    leftTurn();
  } else if (fB > fA) {
    // {fB is louder}
    Serial.println("RESULT: 3kHz louder than 300Hz");
//    rightTurn();
  } else if (fA > fB) {
    // {fA is louder}
    Serial.println("RESULT: 300Hz louder than 3kHz");
//    uTurn();
  }
  delay(500);
}
