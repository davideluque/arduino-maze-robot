#include <Servo.h> 
#include <HCSR04.h>
#include <QTRSensors.h> // Pololu QTR Sensor Library.
//based on Reflectance Sesors's 1~6 connected to Arduino's 8~3
//有的时候能用，用的时候不能，为什么？不好的的时候，甚至都不拐弯，直接撞墙。

Servo left_motor, right_motor;
const int TrigPin = 9;
const int EchoPin = 10;
const int left_motorPin = 11;
const int right_motorPin = 12;
UltraSonicDistanceSensor distanceSensor(TrigPin, EchoPin);
double distance;

#define KP 0.1 //experiment to determine this, start by something sm  all that just makes your bot follow the line at a slow speed
#define KD 0.7 //experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define left_motor_default_speed 180  //normal speed of the left_motor
#define right_motor_default_speed 180  //normal speed of the right_motor
#define left_motor_max_speed 180 //max. speed of the left_motor
#define right_motor_max_speed 180 //max. speed of the right_motor

#define NUM_SENSORS  6        //number of sensors used
#define TIMEOUT 2500          //waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 8         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 2
      

// sensors are connected to digital pins 2 to 7
QTRSensorsRC qtrrc((unsigned char[]) { 3, 4, 5, 6, 7, 8},  // 3号传感器对应最左边or最右边要测清楚？
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
int error = 0;
int lastError = 0;
int type = 0;

void setup() {
  Serial.begin(9600); // 波特率
  left_motor.attach(left_motorPin);
  right_motor.attach(right_motorPin);
  set_motors(90,90); // set the motors not moving
  delay(1500);
  manual_calibration();  
}



void loop() {
  distance = distanceSensor.measureDistanceCm();   
 
  
  //1, 2, 3, 4, 5, 6 corresonds to leftonly, rightonly, T, 十, leftandstraight,
  //rightandstraight
  int position = qtrrc.readLine(sensorValues);
  Serial.print(landscape());
  Serial.print('\n');
  /*
  switch(type){
    case 1:
    {
      test_blink(1);
      break;
      } 
    case 2:{
      test_blink(2);
      break;
      }
    case 3: {
      test_blink(3);
      break;
      }
    case 4:{
      test_blink(4);
      break;
      }
    case 5:{
      test_blink(5);
      break;
      }
    case 6:{
      test_blink(6);
      break;
      }
    case 7:  break; // 不考虑 U turn的话，应该是直线，交给PID
    }
 */
 // dead end should be checked if there's a wall or line removed
 //check_dead_end();
 
  int error = -2500 + position;   
   //换掉左右不仅改变了正负，还改变了每个传感器的权值，要么改Kp， Kd,要么需要把后面左右判断全改了
  int motorSpeed = KP * error + KD * (error - lastError);  
  lastError = error;
    
  int leftMotorSpeed = 180 - (left_motor_default_speed + motorSpeed); // closer to 0, speed up
  int rightMotorSpeed = right_motor_default_speed - motorSpeed; // closer to 180, speed up, so slow down here

  set_motors(leftMotorSpeed, rightMotorSpeed); 
}

void set_motors(int left_motorspeed, int right_motorspeed) {
  // limit motor's speed to 0~90, 90~180
  if (180 - left_motorspeed > left_motor_max_speed ) left_motorspeed = 180 - left_motor_max_speed;  // if speed is negative, use 0 instead
  if (right_motorspeed > right_motor_max_speed ) right_motorspeed = right_motor_max_speed;
  // there's no need to ban spin backward, it allows it turn left or right better.
  //if ( left_motorspeed >= 90) left_motorspeed = 90;   // avoid spinning backward
  //if (right_motorspeed <= 90) right_motorspeed = 90;
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
}


int landscape(){
  // first make sure the car is online, then judge the landscape
  // use ultra sound sensors to distinguish 'only right' and 'right or straight'
  // also for 'T' and '十'
  //1, 2, 3, 4, 5, 6 corresonds to leftonly, rightonly, T, 十, leftandstraight,
  //rightandstraight 
  //int position = qtrrc.readLine(sensorValues);
  // for debugging
   for (unsigned int i = 0;i < NUM_SENSORS; i++){
    Serial.print(i);
    Serial.print(' ');
    Serial.print(sensorValues[i]);
    Serial.print('\t'); 
  }
  
    // 实际中需要往前跑一段再判断
    
    if(left_judge()){
      Serial.print(666);
      Serial.print(' ');
      if (wall_judge()) return 1;  // use true instead of wall_judge()
      else return 5;
      } 
      
    if(right_judge()) {
      Serial.print(777);
      Serial.print(' ');
      if (wall_judge()) return 2;
      else return 6;
      }

    if(T_judge()){
      Serial.print(888);
      Serial.print(' ');
      if (wall_judge()) return 3;
      else return 4;
      }
      
    return 7; // 用于调试
}


bool sensor_judge(unsigned int num, int range, int error){
  int a = sensorValues[num];
 if ((a <= range + error) && (a >= range - error)) return true;
  else return false; 
  }

bool left_judge(){
  // tell onlyleft from 'left and straight'
  // the sensor values should be {1000, 1000, 1000, 1000, 0, 0}， 1号传感器对应的sensorValues[5]
  Serial.print(12);
  Serial.print(' ');
  if ((sensorValues[0]!= 0) && (sensorValues[1] != 0) && (sensorValues[2] != 0) && (sensorValues[3] != 0)\
  && (sensorValues[4] == 0) && (sensorValues[5] == 0)) return true;
  else if ((sensorValues[0] != 0) && (sensorValues[1] != 0) && (sensorValues[2] != 0) && (sensorValues[3] == 0)\
  && (sensorValues[4] == 0) && (sensorValues[5] == 0)) return true;
  else if ((sensorValues[0] != 0) && (sensorValues[1] != 0) && (sensorValues[2] != 0) && (sensorValues[3] != 0)\
  && (sensorValues[4] != 0) && (sensorValues[5] == 0)) return true;
  else return false;/*
  bool a = sensor_judge(0, 650, 350) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);
  
  bool c = sensor_judge(0, 650, 350) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool d = sensor_judge(3, 125, 125) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);

  bool e = sensor_judge(0, 650, 350) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool f = sensor_judge(3, 650, 350) && sensor_judge(4, 650, 350) && sensor_judge(5, 125, 125);
  //按正常接线顺序，sensor_judge的顺序是0-5
  return (a && b) || (c && d) || (e && f);*/
  }

bool right_judge(){
  // tell onlylright from 'right and straight'
  // the sensor values should be{0, 0, 1000, 1000, 1000, 1000}
  Serial.print(34);
  Serial.print(' ');
  if ((sensorValues[0] == 0) && (sensorValues[1] == 0) && (sensorValues[2] != 0) && (sensorValues[3] != 0)\
  && (sensorValues[4] != 0) && (sensorValues[5] != 0)) return true;
  else if ((sensorValues[0] == 0) && (sensorValues[1] == 0) && (sensorValues[2] == 0) && (sensorValues[3] != 0)\
  && (sensorValues[4] != 0) && (sensorValues[5] != 0)) return true;
  else if ((sensorValues[0] == 0) && (sensorValues[1] != 0) && (sensorValues[2] != 0) && (sensorValues[3] != 0)\
  && (sensorValues[4] != 0) && (sensorValues[5] != 0)) return true;
 else return false;/*
  
  bool a = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 350, 650) && sensor_judge(5, 650, 350);

  bool c = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, 125, 125);
  bool d = sensor_judge(3, 650, 350) && sensor_judge(4, 650, 350) && sensor_judge(5, 650, 350);

  bool e = sensor_judge(0, 125, 125) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool f = sensor_judge(3, 650, 350) && sensor_judge(4, 650, 350) && sensor_judge(5, 650, 350);
   return (a && b) || (c && d) || (e && f);*/
  }

bool T_judge(){
  // tell T from 十
  // the sensor values should be all 1000
  Serial.print(56);
  Serial.print(' ');
 if ((sensorValues[0] != 0) && (sensorValues[0] != 0) && (sensorValues[0] != 0) && (sensorValues[0] != 0)\
  && (sensorValues[0] != 0) && (sensorValues[0] != 0)) return true;
  else return false;/*
  bool a = sensor_judge(0, 650, 350) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 650, 350) && sensor_judge(5, 650, 350);
  return a && b; */
  }


bool wall_judge(){
  // use ultra_sound to judge if there is a wall in fornt
  // return true if there's a wall in the front  

    if((distance>10) && (distance<20)){
      return true;
    }
      
    else{
      return false;
      }
           
  }


void right_turn(){
  left_motor.write(0);  //左轮正转
  right_motor.write(0);  //右轮反转
  delay(2150/2);
  return;
}

void left_turn(){
  left_motor.write(180);  //左轮正转
  right_motor.write(180);  //右轮反转
  delay(2150/2);
  return;
}

void go_straight(){
  left_motor.write(0);  //左轮正转
  right_motor.write(180);  
  delay(1000);
  return;
  }

void test_blink(int number){
      for (int i = 0; i< number; i ++){
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
        delay(1000);
      }
      return;
  }

/*
bool check_reflectance_dead_end(int position){
  int lower_bound = 2300;
  int higher_bound = 2700;
  if (position > 2300 and position < 2700){
    return true;
  }
  return false;
}

void check_u_turn(int position){
  u_turn = false; // we assume we shouldn't do u-turn
  // stage1: check for reflectance bounds in a fixed range (determined by testing)
  u_turn = check_reflectance_dead_end(position);
  // stage2: check for ultra-sound sensor (maybe a variable that has been set before)

  return u_turn;
}

void u_turn(){
  motor1.write(0);  //左轮正转
  motor2.write(0);  //右轮反转
  delay(2150);
  return;
}*/
