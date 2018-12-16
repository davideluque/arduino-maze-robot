/*
 * Main Module
*/

int k;

void loop(){
    follow_line();
    //rescue_first();
    k++;
    //1, 2, 3, 4, 5, 6 corresonds to leftonly, rightonly, T, 十, leftandstraight,
    //rightandstraight
    //int position = qtrrc.readLine(sensorValues);  
    if (k>200){
        Serial.print(landscape());
        Serial.print('\n');
    }
}

void setup(){
    Serial.begin(9600);
    setup_linefollowing();
    setup_driving();
    //setup_gripping();
    delay(500);
}
