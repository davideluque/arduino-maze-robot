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
  left_motor.write(left_motorspeed);
  right_motor.write(right_motorspeed);
}

void go_straight(){
    left_motor.write(0);  //左轮正转
    right_motor.write(180);
    delay(350);
    return;
}

void right_turn(){
  Serial.println("I am going to turn right");
  left_motor.write(0);  //左轮正转
  right_motor.write(0);  //右轮反转
  Serial.println("I am going to delay while I do the turn");
  delay(800);
  Serial.println("Turn should have finished");
  return;
}

void left_turn(){
    left_motor.write(180);  //左轮正转
    right_motor.write(180);  //右轮反转
    delay(800);
    return;
}

void u_turn(){
    left_motor.write(0);  //左轮正转
    right_motor.write(0);  //右轮反转
    delay(1600);
    return;
}
