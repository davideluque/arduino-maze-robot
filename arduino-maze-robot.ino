/*
 * 
 */
#include <Servo.h>
#include <QTRSensors.h> // Pololu QTR Sensor Library.

Servo left_motor, right_motor;
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
#define EMITTER_PIN 7         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 2
#define DEBUG 0

// sensors are connected to digital pins 1 to 6
QTRSensorsRC qtrrc((unsigned char[]) {1, 2, 3, 4, 5, 6},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
int lastError = 0;

void setup() {
  left_motor.attach(left_motorPin);
  right_motor.attach(right_motorPin);
  set_motors(90, 90); // set the motors speed zero.
  manual_calibration();
}

void loop() {
  int position = qtrrc.readLine(sensorValues); //get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.

  if (DEBUG) print_sensor_values(position);

  int error = position - 2500;
  
  if (DEBUG) print_error(error);

  int motorSpeed = KP * error + KD * (error - lastError);
  
  if (DEBUG) print_motorSpeed(motorSpeed);

  lastError = error;

  int leftMotorSpeed = 180 - left_motor_default_speed - motorSpeed; // closer to 0, speed up
  int rightMotorSpeed = right_motor_default_speed - motorSpeed; // closer to 180, speed up, so slow down here
  
  if (DEBUG) print_left_right_motorSpeed(leftMotorSpeed, rightMotorSpeed);

  // set motor speeds using the two motor speed variables above
  // limit their speed to 90~180
  set_motors(leftMotorSpeed, rightMotorSpeed);
  //set_motors(90, 90);
  //set_motors(180, 180);
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