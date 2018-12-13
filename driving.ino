/*
 *
 * Driving module
 *  
*/ 

#include <Servo.h>

#define left_motor_max_speed 180 
#define right_motor_max_speed 180

Servo left_motor, right_motor;

void setup_driving(){
    // Attach driving system left and right motor to arduino pins 11 & 12.
    left_motor.attach(11);
    right_motor.attach(12);

    // Put left and right motor in rest mode (not moving)
    left_motor.write(90);
    right_motor.write(90);
    return;
}

void set_motors(int left_motorspeed, int right_motorspeed) {
  if (180 - left_motorspeed > left_motor_max_speed ) left_motorspeed = 180 - left_motor_max_speed;  // if speed is negative, use 0 instead
  if (right_motorspeed > right_motor_max_speed ) right_motorspeed = right_motor_max_speed;
  if ( left_motorspeed > 90) left_motorspeed = 90;   // avoid spinning backward
  if (right_motorspeed < 90) right_motorspeed = 90;
  //left_motorspeed = 180 - left_motorspeed;
  left_motor.write(left_motorspeed);
  right_motor.write(right_motorspeed);
}

void move(){
    left_motor.write(90);
    right_motor.write(90);
}