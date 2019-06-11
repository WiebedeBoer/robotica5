//
// Created by Wouter Nauta on 2019-06-07.
//

#include "Node.h"

Node::Node(int _angle, Servo& _servo, Node* _last) : ServoArm(_servo), Last(_last) {
    Angle = _angle;
}

double Node::GetLength() {
    if(Last == nullptr){
        return (cos(Angle) * ServoArm.LENGTH()) + Last->GetLength();
    }else{
        return (cos(Angle) * ServoArm.LENGTH());
    }
}