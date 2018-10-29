#include "MeMCore.h"
#define OPSPD 250 // Operating Speed
#define TDURATION 360 // Standard time taken for 90 degree turn

//Global variables
MeDCMotor leftWheel(M1);
MeDCMotor rightWheel(M2); 

//Function prototypes for those with default arguments
void moveForward(short duration = 200);
void leftTurn(uint8_t turnSpeed = OPSPD, short duration = TDURATION);
void rightTurn( uint8_t turnSpeed = OPSPD, short duration = TDURATION);

void setup()
{
}

void loop(){
  moveForward();
  leftTurn();
  moveForward();
  rightTurn();
  moveForward();
  uTurn();
  moveForward();
  LLTurn();
  moveForward();
  RRTurn();
  delay(5000);
}

//General movement functions
void stopWheels(){
  leftWheel.stop();
  rightWheel.stop();
}

void moveForward(short duration = 200){
  leftWheel.run(-OPSPD);
  rightWheel.run(OPSPD);
  delay(duration);
}

void leftTurn(uint8_t turnSpeed = OPSPD, short duration = TDURATION){
  leftWheel.run(turnSpeed);
  rightWheel.run(turnSpeed);
  delay(duration);
  stopWheels();
}

void rightTurn(uint8_t turnSpeed = OPSPD, short duration = TDURATION){ //should this be rewritten as leftTurn with -ve turnSpeed?
  leftWheel.run(-turnSpeed);
  rightWheel.run(-turnSpeed);
  delay(duration);
  stopWheels();
}

void uTurn(){ //should this be rewritten in terms of 2x left or right turns with 0 delay?
  leftWheel.run(OPSPD);
  rightWheel.run(OPSPD);
  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(){
  leftTurn();
  moveForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  leftTurn();
}

void RRTurn(){
  rightTurn();
  moveForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  rightTurn();
}


