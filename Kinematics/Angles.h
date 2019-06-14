//
// Created by Wouter Nauta on 2019-06-11.
// altered by Wiebe de Boer 2019-06-12

#ifndef KINEMATICS_ANGLES_H
#define KINEMATICS_ANGLES_H

#include <math.h>
class Angles {
public:
    double Gonio(double TipX, double TipY);
    double LawOfCosines(double& _a, double& _b, double& _c);
    double Distance(double& _x, double& _y);
	int AngleToPin(double Angle, int Servo);
private:
    double A1; //root joint
    double A2; //counter clockwise joint
	double Len1 = 150.0; //arm segment 1
	double Len2 = 196.0; //arm segment 2
    double D1; //dividing angle A1 joint
    double D2; //dividng angle A1 joint
    double Dist; //distance between tip of segment 2 and root joint
    double TipX; //x coordinate of tip
    double TipY; //y coordinate of tip
	int AnglePinOutput1;
	int AnglePinOutput2;
};


#endif //KINEMATICS_ANGLES_H
