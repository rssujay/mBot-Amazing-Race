#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>

MeDCMotor motor_9(9);
MeDCMotor motor_10(10);		

void move(int direction, int speed)
{
      int leftSpeed = 0;
      int rightSpeed = 0;
      if(direction == 1){
        	leftSpeed = speed;
        	rightSpeed = speed;
      }else if(direction == 2){
        	leftSpeed = -speed;
        	rightSpeed = -speed;
      }else if(direction == 3){
        	leftSpeed = -speed;
        	rightSpeed = speed;
      }else if(direction == 4){
        	leftSpeed = speed;
        	rightSpeed = -speed;
      }
      motor_9.run((9)==M1?-(leftSpeed):(leftSpeed));
      motor_10.run((10)==M1?-(rightSpeed):(rightSpeed));
}
				
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
double red;
double green;
double blue;
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);



void setup(){
    rgbled_7.setColor(0,60,0,0);
    rgbled_7.show();
    _delay(0.1);
    red = lightsensor_6.read();
    rgbled_7.setColor(0,0,60,0);
    rgbled_7.show();
    _delay(0.1);
    green = lightsensor_6.read();
    rgbled_7.setColor(0,0,0,60);
    rgbled_7.show();
    _delay(0.1);
    blue = lightsensor_6.read();
    if(200 < red) && 200 < green && 200 < blue){
        move(2,100);
    } else if((red) > (180)){
            move(3,100);
    } else if((green) > (180)){
            move(3,100);
    } else if((blue) > (180)){
            move(4,100);
            move(1,100);
            _delay(1);
            move(4,100);
    } else if(((red) < (40)) && (((green) < (40)) && ((blue) < (40)))){
            move(4,100);
            move(4,100);
    } else if(((blue) < (40)) && ((135) < (green)) && ((green) < (175)) && ((120) < (red))){
            move(3,100);
            move(1,100);
            _delay(1);
            move(3,100);
    }
}

void loop(){
    
    
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    
}
