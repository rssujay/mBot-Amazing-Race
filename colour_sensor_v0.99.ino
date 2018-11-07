#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>
MeDCMotor motor_9(9);
MeDCMotor motor_10(10);
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);

// global variables
float red_value;
float blue_value;
float green_value;
void colourAction(float red, float green, float blue);

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
void colourAction(float red, float green, float blue) {
  // scan red
  rgbled_7.setColor(0,60,0,0);
  rgbled_7.show();
  delay(1000);
  red = lightsensor_6.read();
  red = red / 1023.0 * 255;
  Serial.print(red);
  Serial.println(" : RED VALUE");

  // scan green
  rgbled_7.setColor(0,0,60,0);
  rgbled_7.show();
  delay(1000);
  green = lightsensor_6.read();
  green = green / 1023.0 * 255;
  Serial.print(green);
  Serial.println(" : GREEN VALUE");
  
  // scan blue
  rgbled_7.setColor(0,0,0,60);
  rgbled_7.show();
  delay(1000);
  blue = lightsensor_6.read();
  blue = blue / 1023.0 * 255;
  Serial.print(blue);
  Serial.println(" : BLUE VALUE");

  Serial.print("THIS IS");
  if (red > 200 && green > 75 && green < 125 && blue < 75) {
    Serial.println(" ORANGE");
  } else if (red > 200 && green > 200 && blue > 200) {
    Serial.println(" WHITE");
  } else if (blue < 40 && green < 40 && blue < 40) {
    Serial.println(" BLACK");
  } else if (red > blue && red > green) {
    Serial.println(" RED");
  } else if (blue > red && blue > green) {
    Serial.println(" BLUE");
  } else if (green > red && green > blue) {
    Serial.println(" GREEN");
  }
}
