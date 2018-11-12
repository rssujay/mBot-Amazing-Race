#include "MeMCore.h"
#define OPSPD 245 // Operating Speed
#define LOSPD 200 // Low Speed
#define TDURATION 290 // Standard time taken for 90 degree turn


//Global variables

MeDCMotor leftWheel(M1); // MOVEMENT
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);
MePort IR(PORT_3);

MeRGBLed rgbled_7(7);  // COLOUR
MeLightSensor lightsensor_6(6);
double red, blue, green,white;

int8_t LIR, RIR; // IR
float leftBound, rightBound;

//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup(){
  leftBound = IR.aRead1()/51.0 - 1.25, rightBound = IR.aRead2()/51.0 - 1.25;
  Serial.begin(9600);
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  delay(300);
  stepForward(50);
  lineFinder.readSensors(); 
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
  colourAction();
}

//General movement functions
void stopWheels(void){
  leftWheel.stop();
  rightWheel.stop();
}
  
void moveForward(void){
  float left = IR.aRead1()/51.0, right = IR.aRead2()/51.0; // divide by 255*5? -- why?
  Serial.print(left);
  Serial.print(", ");
  Serial.print(right);
  Serial.print("\n");

  if (right < rightBound){
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);

  }
  else if (left < leftBound){
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
  stepForward(700); //replace with dynamic value from get_duration (from frontal US emitter/detectors)?
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  stepForward(700);
  rightTurn();
}

void colourAction() {
  // scan
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(1000);
  white = lightsensor_6.read();
  Serial.print(white);
  Serial.println(" : WHITE VALUE");
  
  // scan red
  rgbled_7.setColor(0,255,0,0);
  rgbled_7.show();
  delay(1000);
  red = lightsensor_6.read();
  Serial.print(red);
  Serial.println(" : RED VALUE");

  // scan green
  rgbled_7.setColor(0,0,255,0);
  rgbled_7.show();
  delay(1000);
  green = lightsensor_6.read();
  Serial.print(green);
  Serial.println(" : GREEN VALUE");
  
  // scan blue
  rgbled_7.setColor(0,0,0,255);
  rgbled_7.show();
  delay(1000);
  blue = lightsensor_6.read();
  Serial.print(blue);
  Serial.println(" : BLUE VALUE");

//  Serial.print("THIS IS");
  if (red > 500 && green > 500 && blue > 500) {
    Serial.println(" WHITE");
     uTurn();
  } else if (red < 200 && green < 200 && blue < 200) {
    Serial.println(" BLACK");
     stopWheels();
     //play_music();
     delay(300);
  } else if (blue > red - 50 && blue > green) {
      Serial.println(" BLUE");
     RRTurn();
  } else if (green > red - 50 && green > blue) {
    Serial.println(" GREEN");
      rightTurn();
  } else if (white < 180) { // one side blocked
    if (red + 20 < 560) {
      Serial.println(" RED");
      leftTurn();
    } else {
    Serial.println(" ORANGE");
      LLTurn();
    }
  } else { // both sides clear (white > 180)
    if (red < 565) {
      Serial.println(" RED");
      leftTurn();
    } else {
      Serial.println(" ORANGE");
      LLTurn(); 
    }
  }
}
