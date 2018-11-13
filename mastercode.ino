#include "MeMCore.h"
#define OPSPD 235 // Operating Speed
#define LOSPD 165 // Low Speed
#define TDURATION 290 // Standard time taken for 90 degree turn


//Global variables
MeDCMotor leftWheel(M1); // MOVEMENT
MeDCMotor rightWheel(M2);
MeLineFollower lineFinder(PORT_2);
MePort IR(PORT_3);
int8_t ULTRASONIC = 12;
int8_t LIR, RIR; // IR
float leftBound, rightBound;
int a=0,b=0;


MeRGBLed rgbled_7(7);  // COLOUR
MeLightSensor lightsensor_6(6);
double red, blue, green,white;

//Function prototypes for those with default arguments
void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);
void rightTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION);

void setup(){
  leftBound = IR.aRead1()/51.0 - 1.0, rightBound = IR.aRead2()/51.0 - 1.0;
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
  Serial.println(ultraSound());
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

int16_t ultraSound(void){
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, LOW);
  pinMode(ULTRASONIC, INPUT);
  return pulseIn(ULTRASONIC, HIGH);
}
  
void moveForward(void){
  float left = IR.aRead1()/51.0, right = IR.aRead2()/51.0; // divide by 255*5? -- why?
  if (right < rightBound){ //crr to left
    b=0;
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);
    a++;
  }
  else if (left < leftBound){ //crr to right
    a=0;
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
    b++;
  }
  else if (a>100){
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
  }
  else if (b>100){
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
  while (dist > 2000 && dist < 4200){
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
  while(ultraSound() > 400 || millis()-time < 800){
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

void colourAction() {
  // scan
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(300);
  white = lightsensor_6.read();
  Serial.print(white);
  Serial.println(" : WHITE VALUE");
  
  // scan red
  rgbled_7.setColor(0,255,0,0);
  rgbled_7.show();
  delay(300);
  red = lightsensor_6.read();
  Serial.print(red);
  Serial.println(" : RED VALUE");

  // scan green
  rgbled_7.setColor(0,0,255,0);
  rgbled_7.show();
  delay(300);
  green = lightsensor_6.read();
  Serial.print(green);
  Serial.println(" : GREEN VALUE");
  
  // scan blue
  rgbled_7.setColor(0,0,0,255);
  rgbled_7.show();
  delay(300);
  blue = lightsensor_6.read();
  Serial.print(blue);
  Serial.println(" : BLUE VALUE");

//  Serial.print("THIS IS");
  if (red > 500 && green > 500 && blue > 500) {
    Serial.println(" WHITE");
     uTurn();
  } else if (red < 350 && green < 350 && blue < 350) {
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
//  } else if (white < 180) { // one side blocked
//    if (red + 20 < 560) {
//      Serial.println(" RED");
//      leftTurn();
//    } else {
//    Serial.println(" ORANGE");
//      LLTurn();
//    }
  } else { // both sides clear (white > 180)
//    if (red < 565) {
      Serial.println(" RED");
      leftTurn();
//    } else {
//      Serial.println(" ORANGE");
//      LLTurn(); 
//    }
  }
}
