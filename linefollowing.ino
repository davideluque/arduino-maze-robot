/*
 * Line following module
 * 
*/

#include <QTRSensors.h>

#define NUM_SENSORS 6

QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8},
                    NUM_SENSORS, 2500, 1);
unsigned int sensorValues[NUM_SENSORS];
unsigned int *calibratedMaximum;
unsigned int *calibratedMinimum;

void setup_linefollowing(){    
    automatic_calibration(1000, 0, &calibratedMinimum, &calibratedMaximum);
}

void automatic_calibration(unsigned int min_value, 
                           unsigned int max_value, 
                           unsigned int **calibratedMinimum, 
                           unsigned int **calibratedMaximum){
    //calibratedMaximum = nullptr;
    //calibratedMinimum = nullptr;
    int i;
    
    // Allocate the arrays if necessary.
    if(*calibratedMaximum == 0){
        *calibratedMaximum = (unsigned int*)malloc(sizeof(unsigned int)*NUM_SENSORS);

        // If the malloc failed, don't continue.
        if(*calibratedMaximum == 0)
            return;

        // Initialize the max and min calibrated values to values that
        // will cause the first reading to update them.

        for(i=0;i<NUM_SENSORS;i++)
            (*calibratedMaximum)[i] = min_value;
    }
    
    if(*calibratedMinimum == 0){
        *calibratedMinimum = (unsigned int*)malloc(sizeof(unsigned int)*NUM_SENSORS);

        // If the malloc failed, don't continue.
        if(*calibratedMinimum == 0)
            return;

        for(i=0;i<NUM_SENSORS;i++)
            (*calibratedMinimum)[i] = max_value;
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