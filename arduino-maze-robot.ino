#include <Servo.h>    //Adafruit Motor Shield Library. First you must download and install AFMotor library
#include <QTRSensors.h> //Pololu QTR Sensor Library. First you must download and install QTRSensors library
//Mert Arduino https://bit.ly/MertArduino

Servo motor1, motor2;
const int motor1Pin = 11;
const int motor2Pin = 12;
//AF_DCMotor motor1(1, MOTOR12_1KHZ ); //create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
//AF_DCMotor motor2(2, MOTOR12_1KHZ ); //create motor #2 using M2 output on Motor Drive Shield, set to 1kHz PWM frequency

#define KP 0.2 //experiment to determine this, start by something sm  all that just makes your bot follow the line at a slow speed
#define KD 0.5 //experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd) 
#define M1_default_speed 180  //normal speed of the Motor1
#define M2_default_speed 180  //normal speed of the Motor2
#define M1_maksimum_speed 180 //max. speed of the Motor1
#define M2_maksimum_speed 180 //max. speed of the Motor2
//#define MIDDLE_SENSOR 2       //number of middle sensor used

#define NUM_SENSORS  6        //number of sensors used
#define TIMEOUT 2500          //waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 7         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 2
#define DEBUG 1

//sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsRC qtrrc((unsigned char[]) {
  1, 2, 3, 4, 5, 6
},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  motor1.attach(motor1Pin);
  motor2.attach(motor2Pin);
  set_motors(90, 90); // set the motors not moving
  delay(1500);
  manual_calibration();
}

int lastError = 0;
int last_proportional = 0;
int integral = 0;

void loop() {
  //unsigned int sensors[2];
  int position = qtrrc.readLine(sensorValues); //get calibrated readings along with the line position, refer to the QTR Sensors Arduino Library for more details on line position.

  /*
    // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
    // 1000 means minimum reflectance, followed by the line position
    for (unsigned char i = 0; i < NUM_SENSORS; i++)
    {
    //Serial.print(sensorValues[i]);
    //Serial.print('\t');
    }
    //Serial.println(); // uncomment this line if you are using raw values
    String pos = "Position: ";
    String fpos = pos + position;
    Serial.println(fpos); // comment this line out if you are using raw values
  */
  int error = -2500 + position;
  /*
    String err = "Error: ";
    String ferr = err + error;
    Serial.println(ferr);
    Serial.println();*/

  int motorSpeed = KP * error + KD * (error - lastError);
  //Serial.println(motorSpeed);

  lastError = error;

  int leftMotorSpeed = 180 - M1_default_speed - motorSpeed; // closer to 0, speed up
  int rightMotorSpeed = M2_default_speed - motorSpeed; // closer to 180, speed up, so slow down here
  //Serial.println(leftMotorSpeed);
  //Serial.println(rightMotorSpeed);

  //delay(10);

  // set motor speeds using the two motor speed variables above
  // limit their speed to 90~180
  set_motors(leftMotorSpeed, rightMotorSpeed);
  //set_motors(90, 90);
  //set_motors(180, 180);
}




void set_motors(int motor1speed, int motor2speed) {
  if (180 - motor1speed > M1_maksimum_speed ) motor1speed = 180 - M1_maksimum_speed;  // if speed is negative, use 0 instrad
  if (motor2speed > M2_maksimum_speed ) motor2speed = M2_maksimum_speed;
  if ( motor1speed > 90) motor1speed = 90;   // avoid spinning backward
  if (motor2speed < 90) motor2speed = 90;
  //motor1speed = 180 - motor1speed;
  motor1.write(motor1speed);
  motor2.write(motor2speed);
}




void manual_calibration() {
  //calibrate for sometime by sliding the sensors across the line,
  //or you may use auto-calibration instead
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  delay(500);

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
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}
