//
// Created by Wouter Nauta on 2019-06-07.
//

#ifndef KINEMATICS_NODE_H
#define KINEMATICS_NODE_H

#include <vector>
#include "math.h"
#include "Servo.h"

// Node is the same as Segment
class Node {
public:
    Node(int _angle, const Servo& _servo, Node* _last);
    Node(int _angle, const Servo& _servo);
    double GetLength();
    double GetHeight();
    int AngleChanges();
    int Angle;
	const Servo& ServoArm;
private:
    Node* Last;
    

};


#endif //KINEMATICS_NODE_H
