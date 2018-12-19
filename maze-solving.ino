/*
 * 
 * Maze Solving Module
 * 
 */

// when following the line if there's a wall in front, try to force a turn.

void spot_cylinder(){
    if(check_for_cylinder){
        grasp_cylinder();
    }
    return;
}

bool cylinder_in_channel(){
    distance = distanceSensor.measureDistanceCm();
    if(distance<105&&distance>2){ // cylinder is detected
        return true;
    }
    return false;
}

void traverse_left(){
    bool section1, section2, section3, section4, section5 = false;
    int lc = 9;

    // In the first T, turn left.
    k = 0;
    while(!section1){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        k++;
        if (k > 200){
          lc = landscape();
        }
        if (lc == 5){
            set_motors(90, 90);
            left_turn();
            go_straight();
            spot_cylinder();
            section1 = true;
            break;
        }
    }

    // In the cross, turn left.
    k = 0;
    while(!section2){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        k++;
        if (k > 200){
          lc = landscape();
        }
        if (lc == 6){
            set_motors(90, 90);
            left_turn();
            go_straight();
            spot_cylinder();
            section2 = true;
            break;
        }
    }

    // Let PID do its thing while we 
    // encounter the cross again.
    k = 0;
    while (!section3){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        k++;
        if (k > 200){
          lc = landscape();
        }
        if (lc == 6){
            Serial.println("I am in cross");
            set_motors(90, 90);
            if (cylinder_in_channel()){
                go_straight();
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
                k = 0;
                while (lc != 9){
                    go_straight();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    k++;
                    if (k > 200){
                        lc = landscape();
                    }
                }
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
                k = 0;
                while (lc != 5){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    k++;
                    if (k > 200){
                     lc = landscape();
                    }
                }
                set_motors(90, 90);
                left_turn();
                go_straight();
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
                k = 0;
                while(lc != 2){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    k++;
                    if (k > 200){
                        lc = landscape();
                    }
                }
                go_straight();
                go_straight();
                spot_cylinder();
                k = 0;
                while(lc != 4){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    k++;
                    if (k > 200){
                        lc = landscape();
                    }
                }
                go_straight();
                go_straight();
                k = 0;
                while(lc != 4){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    k++;
                    if (k > 200){
                        lc = landscape();
                    }
                }
                go_straight();
                spot_cylinder();
                section3 = true;
                // Solve the line removed... that is, traverse right.
                return;
            }
            else{
                left_turn();
                go_straight();
                spot_cylinder();
                section3 = true;
                break;
            }
        }
    }

    // Go through all the channel until you find the T.
    k = 0;
    while(!section4){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        k++;
        if (k > 200){
            lc = landscape();
        }
        if (lc == 5){
            Serial.println("I am in T");
            set_motors(90, 90);
            left_turn();
            go_straight();
            spot_cylinder();
            section4 = true;
            break;
        }
    }

    // PID will manage to do the dead end, after that
    // force it to go straight and try to solve no line 
    // case.
    k = 0;
    while(!section5){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        k++;
        if (k > 200){
         lc = landscape();
        }
        if (lc == 4){
            set_motors(90, 90);
            distance = distanceSensor.measureDistanceCm();
            spot_cylinder();
            lc = landscape();
            k = 0;
            while (lc != 8){
                go_straight();
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                k++;
                if (k > 200){
                    lc = landscape();
                }
            }
            break;
        }
    }

    // Solve the line removed... that is, traverse right.
    return;
}


void rescue_first(){
    bool rescued = false;
    int turn_right_times = 0;
    bool in_home = false;
    int turn_left_times = 0;
    int lc = 9;

    Serial.println("Begin rescue known cylinder");
    while (!rescued){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        if (lc == 5 && turn_right_times < 2){
            set_motors(90, 90);
            right_turn();
            go_straight();
            turn_right_times++;
            continue;
        
        }
        // Call Eric's function for detecting people in front
        // and in that function is people is in front, we grasp
        // it and change rescued = true, gripped_first = true
        // and do u-turn.
        if (check_for_cylinder()){
            Serial.println("I found a cylinder");
            grasp_cylinder();
            rescued = true;
            u_turn();
            break;
        }
    }

    Serial.println("Going home..");
    // Loop that goes home.
    while (!in_home){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        if (lc == 2 && turn_left_times < 2){
            set_motors(90, 90);
            left_turn();
            go_straight();
            turn_left_times++;
            continue;
        }
        else if (lc == 7 && turn_left_times == 2){
            // Leave the person in the floor
            in_home = true;
            return;
        }
    }
    return;
}
