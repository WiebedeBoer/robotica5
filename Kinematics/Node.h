//
// Created by Wouter Nauta on 2019-06-07.
//

#ifndef KINEMATICS_NODE_H
#define KINEMATICS_NODE_H

#include <vector>
#include "math.h"
#include "Servo.h"

class Node {
public:
    Node(int _angle, const Servo& _servo, Node* _last);
    Node(int _angle, const Servo& _servo);
    ~Node();
    double GetLength();
    double GetHeight();
    int AngleChanges();
    int Angle;
private:
    Node* Last;
    const Servo& ServoArm;

};


#endif //KINEMATICS_NODE_H
