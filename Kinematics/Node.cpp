//
// Created by Wouter Nauta on 2019-06-07.
//

#include "Node.h"

Node::Node(int _angle, const Servo& _servo, Node* _last) : ServoArm(_servo), Last(_last) {
    Angle = _angle;
}
Node::Node(int _angle, const Servo& _servo) : ServoArm(_servo), Last(nullptr) {
    Angle = _angle;
}
Node::~Node() {
    delete Last;
}
double Node::GetLength() {
    if(Last == nullptr){
        return (cos(Angle) * ServoArm.Length) + Last->GetLength();
    }else{
        return (cos(Angle) * ServoArm.Length);
    }
}
