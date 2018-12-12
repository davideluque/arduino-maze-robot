/*
 *
 * Driving module
 *  
*/ 

#include <Servo.h>

Servo left, right;

void init_driving(){
    left.write(90);
    right.write(90);
    return;
}

void setup_driving(){
    left.attach(11);
    right.attach(12);
    init_driving();
}
