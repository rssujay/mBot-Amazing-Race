#include "MeMCore.h"
#define OPSPD 250 // Operating Speed
#define TDURATION 360 // Standard time taken for 90 degree turn

//Global variables
MeDCMotor leftWheel(M1);
MeDCMotor rightWheel(M2); 

//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup()
{
}

void loop(){
  stepForward(1000);
  leftTurn();
  stepForward(1000);
  rightTurn();
  stepForward(1000);
  uTurn();
  LLTurn();
  RRTurn();
  delay(5000);
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

void uTurn(){ //should this be rewritten in terms of 2x left or right turns with 0 delay?
  leftWheel.run(OPSPD);
  rightWheel.run(OPSPD);
  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(){
  leftTurn();
  stepForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  leftTurn();
}

void RRTurn(){
  rightTurn();
  stepForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  rightTurn();
}
