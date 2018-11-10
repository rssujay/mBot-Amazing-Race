#include <math.h>
#define SND_THSHOLD 100

void setup() {
  pinMode(A0, INPUT);   // 300Hz
  pinMode(A1, OUTPUT);  // 3000Hz
}

void soundSense() {
  int fA = analogRead(A0);  // 300Hz
  int fB = analogRead(A1);  // 3000Hz

  if (abs(fA - fB) < SND_THSHOLD) {
    // {fA & fB same loudness)
    leftTurn();
  } else if (fB > fA) {
    // {fB is louder}
    rightTurn();
  } else if (fA > fB) {
    // {fA is louder}
    uTurn();
  }
}

// pseudocode
//void setup() {
//  pinMode(pin1, input)
//  pinMode(pin2, input)//
//}
//
//void loop() {
//  fa = analogRead(pin1) // (fa is 300hz)
//  fb = analogRead(pin2) // (fb is 3000hz)
//
//if abs(fa - fb) > threshold // threshold is variance of loudness between fa and fb
//    // same loudness
//    left turn
//  else if abs(fb - fa) > 100
//    // fb is louder than fa
//    right turn  
//  else if abs(fa - fb) < 100
//    // fa is louder than fb
//   uturn
}
