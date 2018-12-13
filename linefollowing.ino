/*
 * Line following module
 * 
*/

#include <QTRSensors.h>

#define NUM_SENSORS 6

QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8},
                    NUM_SENSORS, 2500);
unsigned int sensorValues[NUM_SENSORS];

void setup_linefollowing(){    
    Serial.begin(9600);
    Serial.println("Automatic calibration");
    automatic_calibration(8, 1025, &qtrrc.calibratedMinimumOff, &qtrrc.calibratedMaximumOff);
    print_calibrated_values();
    //manual_calibration();
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

    print_sensor_values(position);
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
        Serial.print(qtrrc.calibratedMinimumOff[i]);
        Serial.print(' ');
    }
    Serial.println();

    // print the calibration maximum values measured when emitters were on
    for (int i = 0; i < NUM_SENSORS; i++){
        Serial.print(qtrrc.calibratedMaximumOff[i]);
        Serial.print(' ');
    }
    Serial.println();
}