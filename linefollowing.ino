/*
 * Line following module
 * 
*/

#include <QTRSensors.h>
#include <HCSR04.h>

#define NUM_SENSORS 6
#define KP 0.2
#define KD 0.5

#define left_motor_default_speed 180  //normal speed of the left_motor
#define right_motor_default_speed 180  //normal speed of the right_motor

#define RangeB 625
#define ErrorB 375

QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8},
                    NUM_SENSORS, 2500, 1);
unsigned int sensorValues[NUM_SENSORS];

UltraSonicDistanceSensor distanceSensor(9, 10);

int lastError = 0;

void setup_linefollowing(){    
    //automatic_calibration(1025, 8, &qtrrc.calibratedMinimumOn, &qtrrc.calibratedMaximumOn);    
    manual_calibration();
    //print_calibrated_values();
    return;
}

void automatic_calibration(unsigned int min_value, 
                           unsigned int max_value, 
                           unsigned int **calibratedMinimum, 
                           unsigned int **calibratedMaximum){
    int i;
    unsigned int max_sensor_values[6];
    unsigned int min_sensor_values[6];
    
    // Allocate the arrays if necessary.
    if(*calibratedMaximum == 0){
        *calibratedMaximum = (unsigned int*)malloc(sizeof(unsigned int)*NUM_SENSORS);

        // If the malloc failed, don't continue.
        if(*calibratedMaximum == 0)
            return;

        // Initialize the max and min calibrated values to values that
        // will cause the first reading to update them.
        for(i=0;i<NUM_SENSORS;i++)
            (*calibratedMaximum)[i] = 0;
    }
    
    if(*calibratedMinimum == 0){
        *calibratedMinimum = (unsigned int*)malloc(sizeof(unsigned int)*NUM_SENSORS);

        // If the malloc failed, don't continue.
        if(*calibratedMinimum == 0)
            return;

        for(i=0;i<NUM_SENSORS;i++)
            (*calibratedMinimum)[i] = 2500;
    }

    
    for(i=0;i<NUM_SENSORS;i++){
        // set the max we found
        // Todo: set this to an array of the average
        // of some readings because between sensor max differs
        // quite.
        max_sensor_values[i] = max_value;

        // set the min we found
        min_sensor_values[i] = min_value;
    }

    // record the min and max calibration values
    for(i=0;i<NUM_SENSORS;i++)
    {
        if(min_sensor_values[i] > (*calibratedMaximum)[i])
            (*calibratedMaximum)[i] = min_sensor_values[i];
        if(max_sensor_values[i] < (*calibratedMinimum)[i])
            (*calibratedMinimum)[i] = max_sensor_values[i];
    }
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

void follow_line(){
    int position = qtrrc.readLine(sensorValues);

    int error = position - 2500;

    //print_sensor_values(position);

    int motorSpeed = KP * error + KD * (error - lastError);
    
    lastError = error;

    int leftMotorSpeed = 180 - left_motor_default_speed - motorSpeed;
    int rightMotorSpeed = right_motor_default_speed - motorSpeed; // closer to 180, speed up, so slow down here

    set_motors(leftMotorSpeed, rightMotorSpeed);
}

void manual_calibration() {
  //calibrate for sometime by sliding the sensors across the line,
  //or you may use auto-calibration instead

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  //void emittersOn();
  //Serial.println("Start calibration");
  for (int i = 0; i < 400; i++){
      qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
  
  print_calibrated_values();
  delay(1000);
}

void print_calibrated_values(){
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
    if(left_judge()){
        k=0;
        if (wall_judge()){
            return 1;
        }
        return 2;
    }
    
    if(right_judge()){
        k=0;
        if (wall_judge()){
            return 3;
        }
        return 4;
      }

    if(T_judge()){
        k=0;
        if (wall_judge()){
            return 5;
        }
        return 6;
    }   

    if(dead_end_judge()){
        k=0;
        if (wall_judge()){
            return 7;
        }
        return 8;
      }
          
    return 9; 
}

bool sensor_judge(unsigned int num, int range, int error){
     int a = sensorValues[num];
     if ((a <= range + error) && (a >= range - error)){
         return true;
     }
     return false; 
}

bool left_judge(){
  // tell onlyleft from 'left and straight'
  // the sensor values should be {1000, 1000, 1000, 1000, 0, 0}， 1号传感器对应的sensorValues[5]
  bool a = sensor_judge(0, RangeB, ErrorB) && sensor_judge(1, RangeB, ErrorB) && sensor_judge(2, RangeB, ErrorB);
  bool b = sensor_judge(3, RangeB, ErrorB) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);
  
  bool c = sensor_judge(0, RangeB, ErrorB) && sensor_judge(1, RangeB, ErrorB) && sensor_judge(2, RangeB, ErrorB);
  bool d = sensor_judge(3, 125, 125) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);

  bool e = sensor_judge(0, RangeB, ErrorB) && sensor_judge(1,RangeB, ErrorB) && sensor_judge(2, RangeB, ErrorB);
  bool f = sensor_judge(3, RangeB, ErrorB) && sensor_judge(4, RangeB, ErrorB) && sensor_judge(5, 125, 125);
  //按正常接线顺序，sensor_judge的顺序是0-5
  return (a && b) || (c && d) || (e && f);
}

bool right_judge(){
  // tell only  right from 'right and straight'
  // the sensor values should be{0, 0, 1000, 1000, 1000, 1000}  
  bool a = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, RangeB, ErrorB);
  bool b = sensor_judge(3, RangeB, ErrorB) && sensor_judge(4, RangeB, ErrorB) && sensor_judge(5,RangeB, ErrorB);

  bool c = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, 125, 125);
  bool d = sensor_judge(3, RangeB, ErrorB) && sensor_judge(4, RangeB, ErrorB) && sensor_judge(5, RangeB, ErrorB);

  bool e = sensor_judge(0, 125, 125) && sensor_judge(1, RangeB, ErrorB) && sensor_judge(2, RangeB, ErrorB);
  bool f = sensor_judge(3, RangeB, ErrorB) && sensor_judge(4, RangeB, ErrorB) && sensor_judge(5, RangeB, ErrorB);
   return (a && b) || (c && d) || (e && f);
}

bool T_judge(){
  // tell T from 十
  // the sensor values should be all 1000
  bool a = sensor_judge(0, RangeB, ErrorB) && sensor_judge(1, RangeB, ErrorB) && sensor_judge(2, RangeB, ErrorB);
  bool b = sensor_judge(3, RangeB, ErrorB) && sensor_judge(4, RangeB, ErrorB) && sensor_judge(5, RangeB, ErrorB);
  return a && b; 
}

bool wall_judge(){
  // use ultra_sound to judge if there is a wall in fornt
  // return true if there's a wall in the front  
  if((distance>10) && (distance<25)){
      return true;
    }    
    return false;
}

bool dead_end_judge(){
  // 全在0附近的区间里
  bool a = sensor_judge(0, 125, 125) && sensor_judge(1, 125, 125) && sensor_judge(2, 125, 125);
  bool b = sensor_judge(3, 125, 125) && sensor_judge(4, 125, 125) && sensor_judge(5, 125, 125);  
  return a && b;
}