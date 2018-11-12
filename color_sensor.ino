#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>		
#define RGBWait 200
#define LDRWait 10


double red;
double green;
double blue;
double white;
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);

//floats to hold colour arrays
float colourArray[] = {0,0,0};

float greyDiff[] = {0,0,0};


void setup(){

  Serial.begin(9600);
}

void loop(){
  colourAction(red, green, blue, white);
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

//  Serial.print("THIS IS");
  if (red > 700 && green > 700 && blue > 700) {
    Serial.println(" WHITE");
//      uTurn();
  } else if (red < 200 && green < 200 && blue < 200) {
    Serial.println(" BLACK");
//      stopWheels();
  } else if (blue > red - 50 && blue > green) {
      Serial.println(" BLUE");
//      RRTurn();
  } else if (green > red - 50 && green > blue) {
    Serial.println(" GREEN");
//      rightTurn();
  } else if (white < 180) { // one side blocked
    if (red + 20 < 560) {
      Serial.println(" RED");
//      leftTurn();
    } else {
    Serial.println(" ORANGE");
//      LLTurn();
    }
  } else { // both sides clear (white > 180)
    if (red < 565) {
      Serial.println(" RED");
//      leftTurn();
    } else {
      Serial.println(" ORANGE");
//      LLTurn(); 
    }
  }
}
