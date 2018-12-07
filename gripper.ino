#include <Servo.h>

Servo gripper, lifter;
const int gripperPin = 6;
const int lifterPin = 7;

void init_lifter(){
  Serial.println("Init lifter");
  lifter.write(180);
  return;
}

void init_gripper(){
  Serial.println("Init gripper");
  gripper.write(0);
  return;
}

void setup() {
  // put your setup code here, to run once:
  gripper.attach(gripperPin);
  lifter.attach(lifterPin);
  Serial.begin(9600);
  init_gripper();
  init_lifter();
  //delay(2000);
}

void lift(){
  Serial.println("Lifting");
  lifter.write(35);
  return;
}

void unlift(){
  Serial.println("Unlifting");
  lifter.write(180);
  return;
}

void open_gripper(){
  Serial.println("Opening gripper");
  gripper.write(0);
  return;
}

void close_gripper(){
  Serial.println("Closing gripper");
  gripper.write(35);
  return;
}

void loop() {
  open_gripper();
  delay(1000);
  close_gripper();
  delay(1000);
}
