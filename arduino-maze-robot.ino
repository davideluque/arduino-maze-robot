/*
 * Main Module
*/

void loop(){
    follow_line();
}

void setup(){
    setup_linefollowing();
    setup_driving();
    //setup_gripping();
    delay(500);
}
