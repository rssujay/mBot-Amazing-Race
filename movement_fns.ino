#include "MeMCore.h"
#define OPSPD 210 // Operating Speed
#define TDURATION 340 // Standard time taken for 90 degree turn

//Global variables
MeDCMotor leftWheel(M1);
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);

//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup()
{
  //start off slower
  moveForward();
}

void loop(){ 
  detectChallenge();
  lineFinder.readSensors();
  delay(1000);
}

// Challenge Solver
void solveChallenge(void){
  rightTurn(); 
  stepForward(500);
}

//Detector functions
bool lineDetect(void){
  int sensorState = lineFinder.readSensors();
  return (sensorState > 2);
}
void detectChallenge(void){ 
  while(!lineDetect()){
    moveForward();
  }
  stopWheels();
  solveChallenge();
}

//General movement functions
void stopWheels(void){
  leftWheel.stop();
  rightWheel.stop();
}

void moveForward(void){
  leftWheel.run(-OPSPD);
  rightWheel.run(OPSPD);
}

void stepForward(uint16_t duration){
  moveForward();
  delay(duration);
  stopWheels();
}

void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION){
  leftWheel.run(turnSpeed);
  rightWheel.run(turnSpeed);
  delay(duration);
  stopWheels();
}

void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION){
  leftTurn(-turnSpeed);
}

void uTurn(void){ //should this be rewritten in terms of 2x left or right turns with 0 delay?
  leftWheel.run(OPSPD);
  rightWheel.run(OPSPD);
  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(void){
  leftTurn();
  stepForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  stepForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  rightTurn();
}
