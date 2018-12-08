/*
 * 
 */
#include <Servo.h>
#include <QTRSensors.h> // Pololu QTR Sensor Library.

Servo left_motor, right_motor;
Servo gripper, lifter;
const int lifterPin = 9;
const int gripperPin = 10;
const int left_motorPin = 11;
const int right_motorPin = 12;

#define KP 0.2 //experiment to determine this, start by something sm  all that just makes your bot follow the line at a slow speed
#define KD 0.5 //experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define left_motor_default_speed 180  //normal speed of the left_motor
#define right_motor_default_speed 180  //normal speed of the right_motor
#define left_motor_max_speed 180 //max. speed of the left_motor
#define right_motor_max_speed 180 //max. speed of the right_motor

#define NUM_SENSORS  6        //number of sensors used
#define TIMEOUT 2500          //waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 8         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 2
#define DEBUG 1

// sensors are connected to digital pins 1 to 6
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
int lastError = 0;

void setup() {
  left_motor.attach(left_motorPin);
  right_motor.attach(right_motorPin);
  gripper.attach(gripperPin);
  lifter.attach(lifterPin);
  set_motors(90, 90); // set the motors speed zero.
  init_gripper();
  init_lifter();
  manual_calibration();
}

void loop() {
  int position = qtrrc.readLine(sensorValues); //get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.

  if(check_u_turn(position)){
    u_turn();
    position = qtrrc.readLine(sensorValues);
  }
  
  
  if (DEBUG) print_sensor_values(position);

  int error = position - 2500;
  
  //if (DEBUG) print_error(error);

  int motorSpeed = KP * error + KD * (error - lastError);
  
  //if (DEBUG) print_motorSpeed(motorSpeed);

  lastError = error;

  int leftMotorSpeed = 180 - left_motor_default_speed - motorSpeed; // closer to 0, speed up
  int rightMotorSpeed = right_motor_default_speed - motorSpeed; // closer to 180, speed up, so slow down here
  
  //if (DEBUG) print_left_right_motorSpeed(leftMotorSpeed, rightMotorSpeed);

  // set motor speeds using the two motor speed variables above
  // limit their speed to 90~180
  set_motors(leftMotorSpeed, rightMotorSpeed);
  //set_motors(90, 90);
  //set_motors(0, 180);
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

void init_lifter(){
  lifter.write(180);
  return;
}

void lift(){
  lifter.write(35);
  return;
}

void unlift(){
  lifter.write(180);
  return;
}

void init_gripper(){
  gripper.write(0);
  return;
}

void open_gripper(){
  gripper.write(0);
  return;
}

void close_gripper(){
  gripper.write(35);
  return;
}

void manual_calibration() {
  //calibrate for sometime by sliding the sensors across the line,
  //or you may use auto-calibration instead

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  //void emittersOn();
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  // delay(1000);
}

void print_sensor_values(int position){
    // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
    // 1000 means minimum reflectance, followed by the line position
    for (unsigned char i = 0; i < NUM_SENSORS; i++){
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
    //Serial.println(); // uncomment this line if you are using raw values
    //String pos = "Position: ";
    //String fpos = pos + position;
    Serial.println(position); // comment this line out if you are using raw values
    return;
}

void print_error(int error){
    String err = "Error: ";
    String ferr = err + error;
    Serial.println(ferr);
    Serial.println();
    return;
}

void print_motorSpeed(int motorSpeed){
  Serial.println(motorSpeed);
  return;
}

void print_left_right_motorSpeed(int leftMotorSpeed, int rightMotorSpeed){
  Serial.println(leftMotorSpeed);
  Serial.println(rightMotorSpeed);  
  return;
}

bool check_reflectance_dead_end(int position){
  if (position == 5000 or position == 0){
    return true;
  }
  return false;
}

bool check_u_turn(int position){
  bool u_turn = false; // we assume we shouldn't do u-turn
  // stage1: check for reflectance bounds in a fixed range (determined by testing)
  u_turn = check_reflectance_dead_end(position);
  // stage2: check for ultra-sound sensor (maybe a variable that has been set before)

  return u_turn;
}

void u_turn(){
  for (int i=0; i<22500;i++){
    left_motor.write(0);
    right_motor.write(0);
  }
  set_motors(90, 90);
  return;
}
