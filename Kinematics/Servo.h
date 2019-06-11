//
// Created by Wouter Nauta on 2019-06-06.
//

#ifndef KINEMATICS_SERVO_H
#define KINEMATICS_SERVO_H


class Servo {
public:
    Servo(int _id, int _minRotation, int _maxRotation, int _angle, int _length);
	int Angle;
    int ID() const { return Id; }
    int MIN_ROTATION() const { return MinRotation; }
    int MAX_ROTATION() const { return MaxRotation; }
    int LENGTH() const {return Length;}
private:
    int Id;
    int MinRotation;
    int MaxRotation;
    int Length;


};


#endif //KINEMATICS_SERVO_H
