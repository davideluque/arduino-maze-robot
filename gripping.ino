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
    gripper.attach(10);
    lifter.attach(9);

    // Put gripper and lifter in rest mode (not moving)
    gripper.write(0);
    lifter.write(180);
    return;
}

bool check_for_cylinder(){
    if(distance<5&&distance>2){ // cylinder is detected
        set_motors(90,90); // stop the car
        return true;
    }
    return false;
}

void grasp_cylinder(){
    //gripper.write(38);
    delay(2000);
    // Not lifting for now.
    //lifter.write(120);
    //delay(2000);
    //gripper.write(5);
    //delay(2000);
    //lifter.write(0);
    //delay(2000);//grasp and lift the cylinder;
}