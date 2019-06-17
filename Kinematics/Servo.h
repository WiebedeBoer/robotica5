//
// Created by Wouter Nauta on 2019-06-06.
//

#ifndef KINEMATICS_SERVO_H
#define KINEMATICS_SERVO_H


class Servo {
public:
    Servo(int _id, int _minRotation, int _maxRotation, int _angle, int _length);
	int Angle;
    const int Id;
    const int MinRotation;
    const int MaxRotation;
    const int Length;
};


#endif //KINEMATICS_SERVO_H
