 /*
 * Main Module
*/

int k;

void loop(){
    //follow_line();
    rescue_first();
    //Serial.println("I am out");
    traverse_left();
    //just_move();
    //k++;
    // if (k>200){
    //     Serial.print(landscape());
    //     Serial.print('\n');
    // }
}

void setup(){
    Serial.begin(9600);
    setup_linefollowing();
    setup_driving();
    setup_gripping();
    delay(500);
}
