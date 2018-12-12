/*
 *
 * Gripping module
 * 
*/

#include <Servo.h>

Servo gripper, lifter;

void init_gripping(){
  gripper.write(0);
  lifter.write(180);
  return;
}

void setup_gripping(){
    gripper.attach(10);
    lifter.attach(9);
    init_gripping();
}