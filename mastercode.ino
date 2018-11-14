#include "MeMCore.h"
#define OPSPD 235 // Operating Speed
#define LOSPD 165 // Low Speed
#define TDURATION 290 // Standard time taken for 90 degree turn
//notes for victory music:
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_E4  330


//Global variables
bool noAction = false; //META

MeDCMotor leftWheel(M1); // MOVEMENT
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);
MePort IR(PORT_3);
int8_t LIR, RIR, a = 0, b = 0, us = 12; // IR + ULTRASONIC
float left, right, leftBound, rightBound; //IR WALL DETECTION

MePort SND(PORT_4); // SOUND

MeRGBLed rgbled_7(7);  // COLOUR
MeLightSensor lightsensor_6(6);
double red, blue, green, white;

//Victory music data:

int melody[] = {NOTE_D4,NOTE_CS4,NOTE_B3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_B3,
                NOTE_B3,NOTE_B3,NOTE_B3,NOTE_A3,NOTE_B3,NOTE_G3,NOTE_G3,NOTE_G3,NOTE_G3,NOTE_G3,
                NOTE_B3,NOTE_B3,NOTE_B3,NOTE_B3,NOTE_B3,NOTE_CS4,NOTE_D4,NOTE_A3,NOTE_A3,NOTE_A3,
                NOTE_A3,NOTE_A3,NOTE_A3,NOTE_D4,NOTE_D4,NOTE_D4,NOTE_D4,NOTE_E4,NOTE_E4,NOTE_CS4};

int noteDurations[] = {2,2,4,4,8,8,8,8,8,8,8,8,4,8,4,3,8,8,8,8,8,8,8,8,4,8,4,3,8,8,8,8,8,8,8,8,4,8,4,3};


//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup(){
  left = IR.aRead1()/51.0 , right = IR.aRead2()/51.0;
  leftBound = max(left - 1,left * 0.925), rightBound = max(right - 1, right * 0.925); //CALIBRATE BOUNDS
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  //delay(300);
  stepForward(50); //START MOVING
  lineFinder.readSensors(); //CALIBRATE SENSORS
}

void loop(){
  if(lineFinder.readSensors() != 3){
    stopWheels();
    solveChallenge();
  }
  moveForward();
}

// Challenge Solver
void solveChallenge(void){
  //soundSense();
  noAction = true;
  if (noAction){
    colourAction();
    noAction = false;
  } 
}

//General movement functions
void stopWheels(void){
  leftWheel.stop();
  rightWheel.stop();
}

int16_t ultraSound(void){
  pinMode(us, OUTPUT);
  digitalWrite(us, LOW);
  delayMicroseconds(2);
  digitalWrite(us, HIGH);
  delayMicroseconds(2);
  digitalWrite(us, LOW);
  pinMode(us, INPUT);
  return pulseIn(us, HIGH);
}
  
void moveForward(void){
  float left = IR.aRead1()/51.0, right = IR.aRead2()/51.0;
  if (right < rightBound){ //correct towards left
    b = 0;
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);
    a++;
  }
  else if (left < leftBound){ //correct towards right
    a = 0;
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
    b++;
  }
  else if (a > 75){ //repay correction debt
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
  }
  else if (b > 75){ //repay correction debt
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);
  }
  else{    
    leftWheel.run(-OPSPD);
    rightWheel.run(OPSPD);
  }
}

void stepForward(uint16_t duration){
  leftWheel.run(-LOSPD);
  rightWheel.run(LOSPD);
  delay(duration);
  stopWheels();
}

void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION){
  leftWheel.run(turnSpeed);
  rightWheel.run(turnSpeed);
  delay(duration);
  stopWheels();
  long dist = ultraSound();
  while (dist > 2000 && dist < 4200){ //distance approx > 1 tile and approx < 2 tiles - ignored when mBot is straight
    leftWheel.run(-OPSPD + 50);
    rightWheel.run(-LOSPD + 50);
    dist = ultraSound();
  }
}

void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION){
  leftWheel.run(-turnSpeed);
  rightWheel.run(-turnSpeed);
  delay(duration);
  stopWheels();
  long dist = ultraSound();
  while (dist > 2000 && dist < 4200){
    leftWheel.run(LOSPD - 50);
    rightWheel.run(LOSPD - 50);
    dist = ultraSound();
  }
}

void uTurn(void){
  float left = IR.aRead1()/51.0, right = IR.aRead2()/51.0;
  if (right > left){
    leftWheel.run(-OPSPD);
    rightWheel.run(-OPSPD);
  }
  else{
    leftWheel.run(OPSPD);
    rightWheel.run(OPSPD);
  }

  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(void){
  leftTurn();
  long t = millis();
  while(ultraSound() > 400 || millis()-t < 800){ //within 900 ms it will cover 1 tile distance
    stepForward(100);
  }
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  long t = millis();
  while(ultraSound() > 400 || millis()-t < 800){
    stepForward(100);
  }
  rightTurn();
}

// Colour based decisionmaking
void colourAction() {  
  // scan red
  rgbled_7.setColor(0,255,0,0);
  rgbled_7.show();
  delay(300);
  red = lightsensor_6.read();

  // scan green
  rgbled_7.setColor(0,0,255,0);
  rgbled_7.show();
  delay(300);
  green = lightsensor_6.read();
  
  // scan blue
  rgbled_7.setColor(0,0,0,255);
  rgbled_7.show();
  delay(300);
  blue = lightsensor_6.read();
  if (red > 600 && green > 600 && blue > 600) { // WHITE
     uTurn();
  } 
  else if (red < 350 && green < 350 && blue < 350) { //BLACK
      play_music();
      exit(0);
  } 
  else if (blue > red - 50 && blue > green) { //BLUE
     RRTurn();
  } 
  else if (green > red - 50 && green > blue) { //GREEN
    rightTurn();
  } 
  else { // RED
    leftTurn();
  }
}

void soundSense() {
  int fL = SND.aRead2();
  int fH = SND.aRead1();
  delay(1000);

  float r = (float) fL / ((float) fH + 0.01);
  if (fL + fH < 150){
    noAction = true;
  }
  else if (r > 100) { // {300Hz > 3kHz}
    leftTurn();
  } 
  else if (r < 0.5) { // {3kHz > 300Hz}
    rightTurn();
  } 
  else if (r >= 0.5 && r <= 100) { // {3kHz same as 300Hz}
    uTurn();
  }
}

void play_music() {
  for (int thisNote = 0; thisNote <= 41; thisNote++) {
    int noteDuration = 1100/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
