/*
 * Main Module
*/

void loop(){
    follow_line();
    //move();
}

void setup(){
    setup_linefollowing();
    setup_driving();
    //setup_gripping();
    delay(500);
}
