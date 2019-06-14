//
// Created by Wouter Nauta on 2019-06-11.
// altered by Wiebe de Boer

#ifndef KINEMATICS_ANGLES_H
#define KINEMATICS_ANGLES_H

#include <math.h>
#include <vector>

class Angles {
public:
    std::vector<int> Gonio(double TipX, double TipY);
    double LawOfCosines(double& _a, double& _b, double& _c);
    double Distance(double& _x, double& _y);
	int AngleToPin(double Angle, int Servo);
private:
	std::vector<int> args;
    double A1; //root joint
    double A2; //counter clockwise joint
	double Len1; //arm segment 1
	double Len2; //arm segment 2
    double D1; //dividing angle A1 joint
    double D2; //dividng angle A1 joint
    double Dist; //distance between tip of segment 2 and root joint
    double TipX; //x coordinate of tip
    double TipY; //y coordinate of tip
	int AnglePinOutput1;
	int AnglePinOutput2;
};


#endif //KINEMATICS_ANGLES_H
