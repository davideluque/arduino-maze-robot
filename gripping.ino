/*
 *
 * Gripping module
 * 
*/

#include <Servo.h>

Servo gripper, lifter;
double distance = 0.0;

void setup_gripping(){
    // Attach gripper and lifter to arduino pins 9 & 10.
    gripper.attach(9);
    lifter.attach(10);

    // Put gripper and lifter in rest mode (not moving)
    gripper.write(88);
    delay(2000);
    lifter.write(0);
    delay(2000);
    return;
}

bool check_for_cylinder(){
    if(distance<4&&distance>2){ // cylinder is detected
        set_motors(90,90); // stop the car
        return true;
    }
    return false;
}

void grasp_cylinder(){
  gripper.write(60);
  delay(1000);   
  lifter.write(115);
  delay(2000);
  gripper.write(130);
  delay(150);
  gripper.write(91);
  delay(2000);
  lifter.write(0);
  delay(2000);
}