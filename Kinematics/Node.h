//
// Created by Wouter Nauta on 2019-06-07.
//

#ifndef KINEMATICS_NODE_H
#define KINEMATICS_NODE_H

#include <vector>
#include "Servo.h"

class Node {
public:
    Node(Node* _last, Servo& _servo);
    double GetLength();
private:
    Node* Last;
    Servo& ServoArm;

};


#endif //KINEMATICS_NODE_H
