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


void rescue_first(){
    bool rescued = false;
    int turn_right_times = 0;
    bool in_home = false;
    int lc = 9;
    int turn_left_times = 0;

    while (!rescued){
        distance = distanceSensor.measureDistanceCm();
        follow_line();
        lc = landscape();
        if (lc == 5 && turn_right_times < 2){
            turn_right_times++;
            right_turn();
        }
        // Call Eric's function for detecting people in front
        // and in that function is people is in front, we grasp
        // it and change rescued = true, gripped_first = true
        // and do u-turn.
        if (turn_right_times == 2 && check_for_cylinder()){
            grasp_cylinder();
            rescued = true;
            u_turn();
            break;
        }
    }

    // Loop that goes home.
    while (!in_home){
        distance = distanceSensor.measureDistanceCm();
        follow_line();
        lc = landscape();
        if (lc == 2 && turn_left_times < 2){
            turn_left_times++;
            left_turn();
        }
        if (lc == 7){
            // Leave the person in the floor
            u_turn();
            return;
        }
    }
    
}