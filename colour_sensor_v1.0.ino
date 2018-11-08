#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);

// global variables
int red_value;
int blue_value;
int green_value;
void colourAction(int red, int green, int blue);

void setup() {
  Serial.begin(9600);
}

void loop() {
  colourAction(red_value, green_value, blue_value);
}

/**
 * What to do when colour sensor detects a colour
 * Detects RGB values and does challenge
 */
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
