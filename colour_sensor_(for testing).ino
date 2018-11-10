#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>		
#define RGBWait 200
#define LDRWait 10

#define OPSPD 210 // Operating Speed
#define TDURATION 340 // Standard time taken for 90 degree turn

//Global variables
MeDCMotor leftWheel(M1);
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);



//Function prototypes for those with default arguments

void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);


double red;
double green;
double blue;
double white;
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);


void setup(){
  lineFinder.readSensors(); //initializes the starting position 
  delay(500); // temporary fix
  moveForward();
  Serial.begin(9600);
}

void loop(){
  detectChallenge();
  lineFinder.readSensors();
  delay(500); //temporary fix
  //colourAction(red, green, blue, white);
}

//Detector functions
void detectChallenge(void){
  while(lineFinder.readSensors() == 3){
    moveForward();
  }
  stopWheels();
  solveChallenge();
}

// Challenge Solver
void solveChallenge(void){
  colourAction(red, green, blue, white);
}

//General movement functions
void stopWheels(void){
  leftWheel.stop();
  rightWheel.stop();
}

void moveForward(void){
  leftWheel.run(-OPSPD);
  rightWheel.run(OPSPD - 5);
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

void uTurn(void){
  leftWheel.run(OPSPD);
  rightWheel.run(OPSPD);
  delay(TDURATION * 2);
  stopWheels();
}

void LLTurn(void){
  leftTurn();
  stepForward(800); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  stepForward(800); //replace with dynamic value from get_duration (from frontal US emitter/detectors)
  rightTurn();
}

void colourAction(int red, int green, int blue, int white) {
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

  Serial.print("THIS IS");
  if (red > 400 && green > 400 && blue > 400) {
    Serial.println(" WHITE");
      uTurn();
  } else if (red < 200 && green < 200 && blue < 200) {
    Serial.println(" BLACK");
    while(1 == 1) {
      stopWheels();
    }
  } else if (blue > red - 50 && blue > green) {
      Serial.println(" BLUE");
      RRTurn();
  } else if (green > red - 50 && green > blue) {
    Serial.println(" GREEN");
      rightTurn();
  } else if (white < 180) { // one side blocked
    if (red + 25 < 565) {
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
