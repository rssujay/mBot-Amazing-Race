#include "MeMCore.h"
#define OPSPD 225 // Operating Speed
#define LOSPD 185 // Low Speed
#define TDURATION 340 // Standard time taken for 90 degree turn

//Global variables
MeDCMotor leftWheel(M1); // MOVEMENT
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);

MeRGBLed rgbled_7(7);  // COLOUR
MeLightSensor lightsensor_6(6);
int16_t red_value, blue_value, green_value;

const int8_t LIR = A2, RIR = A3; // IR

//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup(){
  lineFinder.readSensors(); 
  Serial.begin(9600);
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  delay(300);
  stepForward(300);
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
  colourAction(red_value, green_value, blue_value);
}

//General movement functions
void stopWheels(void){
  leftWheel.stop();
  rightWheel.stop();
}

void moveForward(void){
  float left = analogRead(LIR)/51.0, right = analogRead(RIR)/51.0; // divide by 255*5? -- why?
 
  if (left - right > 3.0){
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);
  }
  else if (right - left > 3.0){
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
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
}

void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION){
  leftTurn(-turnSpeed, duration);
}

void uTurn(void){
  leftWheel.run(OPSPD);
  rightWheel.run(OPSPD);
  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(void){
  leftTurn();
  stepForward(500); //replace with dynamic value from get_duration (from frontal US emitter/detectors)?
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  stepForward(500);
  rightTurn();
}

void colourAction(int red, int green, int blue) {
  // scan red
  rgbled_7.setColor(0,60,0,0);
  rgbled_7.show();
  delay(1000);
  red = lightsensor_6.read();
//  Serial.print(red);
//  Serial.println(" : RED VALUE");

  // scan green
  rgbled_7.setColor(0,0,60,0);
  rgbled_7.show();
  delay(1000);
  green = lightsensor_6.read();
// Serial.print(green);
// Serial.println(" : GREEN VALUE");
  
  // scan blue
  rgbled_7.setColor(0,0,0,60);
  rgbled_7.show();
  delay(1000);
  blue = lightsensor_6.read();
//  Serial.print(blue);
//  Serial.println(" : BLUE VALUE");

//  Serial.print("THIS IS");
  if (red > 350 && red < 450 && green > 250 && green < 350 && blue > 250 && blue < 350) {
//    Serial.println(" ORANGE");
      LLTurn();
  } else if (red > 350 && red < 450 && green > 350 && green < 450 && blue > 350 && blue < 450) {
//    Serial.println(" WHITE");
      uTurn();
  } else if (blue < 200 && green < 200 && blue < 200) {
//    Serial.println(" BLACK");
      stopWheels();
  } else if (red > blue && red > green) {
//    Serial.println(" RED");
      leftTurn();
  } else if (blue > red && blue > green) {
//    Serial.println(" BLUE");
      RRTurn();
  } else if (green > red && green > blue) {
//    Serial.println(" GREEN");
      rightTurn();
  }
}
