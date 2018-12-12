/*
 *
 * Driving module
 *  
*/ 

#include <Servo.h>

Servo left, right;

void setup_driving(){
    // Attach gripper left and right motor to arduino pins 11 & 12.
    left.attach(11);
    right.attach(12);

    // Put gripper and left and right motor in rest mode (not moving)
    left.write(90);
    right.write(90);
    return;
}
