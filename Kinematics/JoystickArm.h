//
// Created by Wouter Nauta on 2019-06-06.
//

#ifndef KINEMATICS_JOYSTICKARM_H
#define KINEMATICS_JOYSTICKARM_H


#include <vector>
#include "Servo.h"
#include "Node.h"

class JoystickArm {
public:
    Node GetPathsVertical();
private:
    const int MIN_HEIGHT = 10;
    const int MAX_HEIGHT = 50;
    const int MAX_LENGTH = 100;

    const std::vector<Servo> VerticalServos = std::vector<Servo>{
            Servo(2, 10, 50, 23, 12),
            Servo(3, 10, 50, 23, 12),
            Servo(4, 10, 50, 23, 12),
            Servo(5, 10, 50, 23, 12)
    };
    const Servo HorizontalServo = Servo(1, 10, 50, 23, 12);
};


#endif //KINEMATICS_JOYSTICKARM_H
