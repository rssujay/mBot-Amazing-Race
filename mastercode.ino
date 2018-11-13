#include "MeMCore.h"
#define OPSPD 235 // Operating Speed
#define LOSPD 165 // Low Speed
#define TDURATION 290 // Standard time taken for 90 degree turn


//Global variables
bool noAction = false; //META

MeDCMotor leftWheel(M1); // MOVEMENT
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);
MePort IR(PORT_3);
int8_t LIR, RIR, a = 0, b = 0, us = 12; // IR + ULTRASONIC
float leftBound, rightBound; //IR WALL DETECTION

MePort SND(PORT_4); // SOUND

MeRGBLed rgbled_7(7);  // COLOUR
MeLightSensor lightsensor_6(6);
double red, blue, green, white;

//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup(){
  leftBound = IR.aRead1()/51.0 - 1.0, rightBound = IR.aRead2()/51.0 - 1.0; //CALIBRATE BOUNDS
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
  soundSense();
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
  else if (a > 100){ //repay correction debt
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
  }
  else if (b > 100){ //repay correction debt
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
  leftWheel.run(OPSPD);
  rightWheel.run(OPSPD);
  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(void){
  leftTurn();
  long time = millis();
  while(ultraSound() > 400 || millis()-time < 800){ //within 900 ms it will cover 1 tile distance
    stepForward(100);
  }
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  long time = millis();
  while(ultraSound() > 400 || millis()-time < 800){
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
  if (red > 500 && green > 500 && blue > 500) { // WHITE
     uTurn();
  } 
  else if (red < 350 && green < 350 && blue < 350) { //BLACK
      //play_music();
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
  if (fL + fH < 100){
    noAction = true;
  }
  else if (r > 100) { // {300Hz > 3kHz}
    leftTurn();
  } 
  else if (r < 0.5) { // {3kHz > 300Hz}
    rightTurn();
  } 
  else if (r > 0.5 && r < 100) { // {3kHz same as 300Hz}
    uTurn();
  }
}
