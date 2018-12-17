/*
 * 
 * Maze Solving Module
 * 
 */

// #include <PriorityQueue.h>

// class Node{
//     private:
//         Node* father;
//         unsigned f, g;
//     public:
//         /* Constructor of Node object.
//         * @param father Pointer to parent.
//         * @param f Represents the approximated cost of the node (f=g+h)
//         * @param g Cost of applying an action to reach this node.
//         */
//         Node(Node* father, unsigned f, unsigned g) { }
//         ~Node() { }

//         unsigned get_f(){
//             return this->f;
//         }

//         unsigned get_g(){
//             return this->g;
//         }
// };

// bool comparef(const Node& a, const Node& b){
//     return *a.get_f() > *b.get_f();
// }

// void a_star(){
//     //PriorityQueue<Node> open_list = PriorityQueue<Node>(comparef);
// }

// when following the line if there's a wall in front, try to force a turn.

void spot_cylinder(){
    if(check_for_cylinder){
        grasp_cylinder();
    }
    return;
}

bool cylinder_in_channel(){
    distance = distanceSensor.measureDistanceCm();
    if(distance<70&&distance>2){ // cylinder is detected
        return true;
    }
    return false;
}

void traverse_left(){
    bool section1, section2, section3, section4, section5 = false;
    int lc = 9;

    // In the first T, turn left.
    while(!section1){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        lc = landscape();
        Serial.println(lc);
        if (lc == 5){
            Serial.println("I am in T");
            set_motors(90, 90);
            left_turn();
            go_straight();
            spot_cylinder();
            section1 = true;
            break;
        }
    }

    // In the cross, turn left.
    while(!section2){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        lc = landscape();
        Serial.println(lc);
        if (lc == 6){
            Serial.println("I am in cross");
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
    while (!section3){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        lc = landscape();
        Serial.println(lc);
        if (lc == 6){
            Serial.println("I am in cross");
            set_motors(90, 90);
            if (cylinder_in_channel()){
                go_straight();
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
                while (lc != 9){
                    go_straight();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    lc = landscape();
                }
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
                while (lc != 5){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    lc = landscape();
                }
                set_motors(90, 90);
                left_turn();
                go_straight();
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
                while(lc != 2){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    lc = landscape();
                }
                go_straight();
                go_straight();
                spot_cylinder();
                while(lc != 4){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    spot_cylinder();
                    lc = landscape();
                }
                go_straight();
                go_straight();
                while(lc != 4){
                    follow_line();
                    distance = distanceSensor.measureDistanceCm();
                    lc = landscape();
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
    while(!section4){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        lc = landscape();
        Serial.println(lc);
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
    while(!section5){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        spot_cylinder();
        lc = landscape();
        Serial.println(lc);
        if (lc == 4){
            set_motors(90, 90);
            distance = distanceSensor.measureDistanceCm();
            spot_cylinder();
            lc = landscape();
            while (lc != 8){
                go_straight();
                distance = distanceSensor.measureDistanceCm();
                spot_cylinder();
                lc = landscape();
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
    int turn_left_times_2 = 0;
    int lc = 9;

    Serial.println("Begin rescue known cylinder");
    while (!rescued){
        follow_line();
        distance = distanceSensor.measureDistanceCm();
        lc = landscape();
        Serial.println(lc);
        if (lc == 5 && turn_right_times < 2){
            Serial.println("I am in T");
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
        if (turn_right_times == 2 && check_for_cylinder()){
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
        lc = landscape();
        if (lc == 2 && turn_left_times < 2){
            set_motors(90, 90);
            left_turn();
            go_straight();
            turn_left_times++;
            continue;
        }
        else if (lc == 1){
          turn_left_times_2++;
          continue;
        }
        else if (lc == 7 && turn_left_times == 2 && turn_left_times_2 == 3){
            // Leave the person in the floor
            return;
        }
    }
    
}
