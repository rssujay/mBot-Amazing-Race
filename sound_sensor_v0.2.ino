#include <math.h>
#define SND_THSHOLD 100
#define SOUND_LW A0 // A0 && A1 are the pins assoicated with the microphone
#define SOUND_HI A1 // use a different pin later

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_LW, INPUT);
  pinMode(SOUND_HI, INPUT);
}

/**
 * SOUND CHALLENGE SOLVER
 */
void soundSense() {
  int fA = analogRead(SOUND_LW);
  int fB = analogRead(SOUND_HI);

  // print fA and fB
  Serial.print("fA: ");
  Serial.println(fA);
  Serial.print("fB: ");
  Serial.println(fB);

  if (abs(fA - fB) < SND_THSHOLD) {
    // {fA & fB same loudness}
    leftTurn();
  } else if (fB > fA) {
    // {fB is louder}
    rightTurn();
  } else if (fA > fB) {
    // {fA is louder}
    uTurn();
  }
}
