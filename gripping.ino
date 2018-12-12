/*
 *
 * Gripping module
 * 
*/

#include <Servo.h>

Servo gripper, lifter;

void setup_gripping(){
    // Attach gripper and lifter to arduino pins 9 & 10.
    gripper.attach(10);
    lifter.attach(9);

    // Put gripper and lifter in rest mode (not moving)
    gripper.write(0);
    lifter.write(180);
    return;
}