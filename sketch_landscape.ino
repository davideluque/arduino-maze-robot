/*
 容纳graspping system的控制函数和line following的控制函数
 */
#include <Servo.h>
#include <HCSR04.h>
#include <QTRSensors.h> // Pololu QTR Sensor Library.

Servo left_motor, right_motor;
Servo gripper, lifter;
const int TrigPin = 9;
const int EchoPin = 10;
const int left_motorPin = 11;
const int right_motorPin = 12;
UltraSonicDistanceSensor distanceSensor(TrigPin, EchoPin);

#define KP 0.2 //experiment to determine this, start by something sm  all that just makes your bot follow the line at a slow speed
#define KD 0.5 //experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define left_motor_default_speed 180  //normal speed of the left_motor
#define right_motor_default_speed 180  //normal speed of the right_motor
#define left_motor_max_speed 180 //max. speed of the left_motor
#define right_motor_max_speed 180 //max. speed of the right_motor

#define NUM_SENSORS  6        //number of sensors used
#define TIMEOUT 2500          //waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 8         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 2
#define DEBUG 0               // 设置为1后，方便后面degbug时要不要打印出来传感器等的参数      

// sensors are connected to digital pins 2 to 7
QTRSensorsRC qtrrc((unsigned char[]) { 3, 4, 5, 6, 7, 8},  // 3号传感器对应最左边or最右边要测清楚？
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
int lastError = 0;

void setup() {
  left_motor.attach(left_motorPin);
  right_motor.attach(right_motorPin);.
  manual_calibration();
}

void loop() {
 Serial.print(landscape());
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

int landscape(){
  // first make sure the car is online, then judge the landscape
  // use ultra sound sensors to distinguish 'only right' and 'right or straight'
  // also for 'T' and '十'
  //1, 2, 3, 4, 5, 6 corresonds to leftonly, rightonly, T, 十, leftandstraight,
  //rightandstraight 
  if(online_judge()){
    delay(1000); // experiment to determine this，实际中这里要不要delay需要商量
    //这里的逻辑需要重新考虑一下，有坑能出现false全部return到第一个里面
    if(left_judge()){
      if (wal_judge()) return 1;
      else return 5;
      } 
      
    if(right_judge() {
      if (wall_judge()) return 1;
      else return 2;
      }

    if(T_judge()){
      if (wall_judge()) return 3;
      else return 4;
      }
    }
  }

//sensorValues是全局变量，不用传值就可调用
bool sensor_judge(num, range, erro){
  // experiment to determine if sensorValues[0] is exact the 1st sensor!!!
  if (sensorValues[num] < range + erro && sensorValues[num] > range - erro) return true;
  else return false; 
  }

bool online_judge(){
   // first judge if the raw vaues array is 0 0 1000 1000 0 0
  // erro is ±250, change it after experiment
  for (unsigned char i = 0;i < NUM_SENSORS; i++){
    if (i != 2 or 3){
      unsigned int range = 0;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false) {
        return false;
        }
      }
     else{
      unsigned int range = 1000;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false){
        return false;
        }
      }
      // there's no false, then return true
       return true;
     }
  }

bool left_judge(){
  // tell onlyleft from 'left and straight'
  // the sensor values should be {1000, 1000, 1000, 1000, 0, 0}
  for (unsigned char i = 0;i < NUM_SENSORS; i++){
    if (i != 4 or 5){
      unsigned int range = 1000;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false) {
        return false;
        }
      }
     else{
      unsigned int range = 0;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false){
        return false;
        }
      }
      // there's no false, then return true
       return true;
     }
  }

bool right_judge(){
  // tell onlylright from 'right and straight'
  // the sensor values should be{0, 0, 1000, 1000, 1000, 1000}
  for (unsigned char i = 0;i < NUM_SENSORS; i++){
    if (i != 1 or 2 ){
      unsigned int range = 10000;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false) {
        return false;
        }
      }
     else{
      unsigned int range = 0;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false){
        return false;
        }
      }
       return true;
     }
  }

bool T_judge(){
  // tell T from 十
  // the sensor values should be all 1000
  for (unsigned char i = 0;i < NUM_SENSORS; i++){
    if (i != 1 or 2 ){
      unsigned int range = 10000;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false) {
        return false;
        }
      }
     else{
      unsigned int range = 0;
      unsigned int erro = 250;
      if (sensor_judge(i, range, erro) == false){
        return false;
        }
      }
       return true;
     }
  }


bool wall_judge(){
  // use ultra_sound to judge if there is a wall in fornt
  // return true if there's a wall in the front  
    double distance = distanceSensor.measureDistanceCm();   
    if(distance<20)
      return true;
    else
      return false;
     delay(200);
  }
