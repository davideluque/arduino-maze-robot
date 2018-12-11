/*
 这版基于reflectance sensor的1-6引脚对应Arduino的3-8引脚，测试通过
 This version is based on reflectance sensor's pin 1-6 coonected to Arduino's digital pin 3-8
 */
//#include <Servo.h>
#include <HCSR04.h>
#include <QTRSensors.h> // Pololu QTR Sensor Library.

//Servo left_motor, right_motor;
//Servo gripper, lifter;
const int TrigPin = 9;
const int EchoPin = 10;
//const int left_motorPin = 11;
//const int right_motorPin = 12;
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
  Serial.begin(9600); // 波特率
//  left_motor.attach(left_motorPin);
//  right_motor.attach(right_motorPin);
  manual_calibration();
  
}

void loop() {
  double distance = distanceSensor.measureDistanceCm();   
  delay(3000);
  landscape();
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
  int position = qtrrc.readLine(sensorValues);
 
  Serial.print(left_judge());
  Serial.print('\n');
  /*
  if(online_judge()){
    // 实际中需要往前跑一段再判断
    if(left_judge()){
      if (wall_judge()) return 1;  // use true instead of wall_judge()
      else return 5;
      } 
      
    if(wall_judge()) {
      if (true) return 2;
      else return 6;
      }

    if(wall_judge()){
      if (true) return 3;
      else return 4;
      }
    }*/
  }


bool sensor_judge(unsigned int num, int range, int error){
  int a = sensorValues[num];
 if ((a <= range + error) && (a >= range - error)) return true;
  else return false;
  }

bool online_judge(){
   // first judge if the raw vaues array is 0 0 1000 1000 0 0
  // erro is ±250, change it after experiment
  for (unsigned int i = 0;i < NUM_SENSORS; i++){
    Serial.print(i);
    Serial.print(' ');
    Serial.print(sensorValues[i]);
    Serial.print('\t'); 
  }  
  bool a = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);
  return a && b;
  }


boolean left_judge(){
  // tell onlyleft from 'left and straight'
  // the sensor values should be {1000, 1000, 1000, 1000, 0, 0}
  for (unsigned int i = 0;i < NUM_SENSORS; i++){
    Serial.print(i);
    Serial.print(' ');
    Serial.print(sensorValues[i]);
    Serial.print('\t'); 
  }
  bool a = sensor_judge(0, 650, 350) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);
  }

bool right_judge(){
  for (unsigned int i = 0;i < NUM_SENSORS; i++){
    Serial.print(i);
    Serial.print(' ');
    Serial.print(sensorValues[i]);
    Serial.print('\t'); 
  }
  // tell onlylright from 'right and straight'
  // the sensor values should be{0, 0, 1000, 1000, 1000, 1000}
  bool a = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 350, 650) && sensor_judge(5, 650, 350);
  return a && b;
  }

bool T_judge(){
  // tell T from 十
  // the sensor values should be all 1000
  for (unsigned int i = 0;i < NUM_SENSORS; i++){
    Serial.print(i);
    Serial.print(' ');
    Serial.print(sensorValues[i]);
    Serial.print('\t'); 
  }
  bool a = sensor_judge(0, 650, 350) && sensor_judge(1, 650, 350) && sensor_judge(2, 650, 350);
  bool b = sensor_judge(3, 650, 350) && sensor_judge(4, 650, 350) && sensor_judge(5, 650, 350);
  return a && b;  
  }


bool wall_judge(double distance){
  // use ultra_sound to judge if there is a wall in fornt
  // return true if there's a wall in the front  
    
    if(distance<10)
      return 1;
    else
      return 0;
     delay(200);
  }
