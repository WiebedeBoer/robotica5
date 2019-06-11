//
// Created by Wouter Nauta on 2019-06-07.
//

#include "Node.h"
#include "Angles.h"

Node::Node(Node* _last, Servo& _servo) : Last(_last), ServoArm(_servo) {
}

double Node::GetLength() {
    if(Last == nullptr){
        // uses cosinus
    }else{

    }
}