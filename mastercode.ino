/* The code below may not compile by default on your machine. This 
 *  is because we have changed the MeMCore.h header file in our compile environment.
 *  
 *  Changes made to MeMCore.h: Commenting out of 
 *  #include "MeInfraredReceiver.h"
 *  #include "MeIR.h"
 *  Both of them were not relevant to our program.
 *  
 *  Reason: These libraries had variable name 
 *  clashes with "MeBuzzer.h" from which the tone() function is used in our code.
 *  
 *  Please do not share the URL indiscriminately!
 *  - SECTION C GROUP 6
 */
 

#include "MeMCore.h"
#define OPSPD 235 // Operating Speed
#define LOSPD 165 // Low Speed
#define TDURATION 270 // Standard time taken for 90-110 degree turn
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
int8_t a = 0, b = 0, us = 12; // IR + ULTRASOUND
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
  leftBound = left - 1.0, rightBound = right - 1.0; //CALIBRATE BOUNDS
  //delay(300);
  stepForward(50); //START MOVING
  lineFinder.readSensors(); //CALIBRATE SENSORS
}

void loop(){
  if(lineFinder.readSensors() < 3 && ultraSound() < 25){
    stopWheels();
    colourAction();
  } 
  moveForward();
}

//General movement functions
void stopWheels(void){
  leftWheel.stop();
  rightWheel.stop();
}

int16_t ultraSound(void){
  pinMode(us, OUTPUT);
  digitalWrite(us, LOW); //Activating the sensor to produce 40 kHz bursts
  delayMicroseconds(2);
  digitalWrite(us, HIGH);
  delayMicroseconds(2);
  digitalWrite(us, LOW);
  pinMode(us, INPUT);
  return pulseIn(us, HIGH); //duration for 2d
}
  
void moveForward(void){
  float currLeft = IR.aRead1()/51.0, currRight = IR.aRead2()/51.0;
  if (currRight < rightBound){ //correct towards left
    b = 0;
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);
    a++;
  }
  else if (currLeft < leftBound){ //correct towards right
    a = 0;
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
    b++;
  }
  else if (a > 125){ //overcorrection; correct towards right
    leftWheel.run(-OPSPD);
    rightWheel.run(LOSPD);
  }
  else if (b > 125){ //overcorrection; correct towards left
    leftWheel.run(-LOSPD);
    rightWheel.run(OPSPD);
  }
  else{ // move straight
    leftWheel.run(-OPSPD);
    rightWheel.run(OPSPD);
  }
}

void stepForward(uint16_t duration){ 
  leftWheel.run(-LOSPD); //Runs at LOSPD in favour of stability
  rightWheel.run(LOSPD);
  delay(duration);
  stopWheels();
}

void leftTurn(int16_t turnSpeed = OPSPD, uint16_t duration = TDURATION){
  leftWheel.run(turnSpeed);
  rightWheel.run(turnSpeed);
  delay(duration); //Experimentally guaranteed to turn > 90 degrees
  stopWheels(); //To allow for accurate ultrasound measurement
  long dist = ultraSound();
  while (dist > 2000 && dist < 4200){ //distance approx > 1 tile and approx < 2 tiles
    leftWheel.run(-OPSPD + 50); //Experimentally determined mild turn
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
  float currLeft = IR.aRead1()/51.0, currRight = IR.aRead2()/51.0; //check current proximity to both side walls, if any
  if (right - currRight < left - currLeft){ //if closer to left wall, make a rightside u-turn 
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
  while(ultraSound() > 550 && millis()-t < 700){ //within 900 ms it will cover 1 tile distance
    stepForward(100);
  }
  leftTurn();
}

void RRTurn(void){
  rightTurn();
  long t = millis();
  while(ultraSound() > 550 && millis()-t < 700){
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
  
  // scan white
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(300);
  white = lightsensor_6.read();
  
  if (red > 600 && green > 600 && blue > 600) { // WHITE
     uTurn();
  } 
  else if (red < 350 && green < 350 && blue < 350) { //BLACK
      soundSense();
      if (noAction){
        play_music();
        exit(0);
      }
  } 
  else if (blue > red - 50 && blue > green) { //BLUE
     RRTurn();
  } 
  else if (green > red - 50 && green > blue) { //GREEN
    rightTurn();
  } 
  else if (white < 180){ // RED
    leftTurn();
  }
  else { // ORANGE
    LLTurn();
  }
}

void soundSense() {
  float lowFreq = SND.aRead2();
  float highFreq = SND.aRead1();
  //delay(1000);

  float ratio = lowFreq / (highFreq + 0.01);
  if (ratio > 50 && ratio <= 5000){
    noAction = true;
  }
  else if (ratio > 5000) { // {300Hz > 3kHz}
    leftTurn();
  } 
  else if (ratio < 0.46) { // {3kHz > 300Hz}
    rightTurn();
  } 
  else { // {3kHz same as 300Hz}
    uTurn();
  }
}

void play_music() {
  while (true){
    for (int thisNote = 0; thisNote <= 39; thisNote++) {
      int noteDuration = 1100/noteDurations[thisNote];
      tone(8, melody[thisNote],noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(8);
    }
  }
}
