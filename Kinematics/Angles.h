//
// Created by Wouter Nauta on 2019-06-11.
//

#ifndef KINEMATICS_ANGLES_H
#define KINEMATICS_ANGLES_H

#include <math.h>
class Angles {
public:
    double Gonio();
    double LawOfCosines(double _a, double _b, double _c);
    double Distance(double _x, double _y);
private:
    double A1; //root joint
    double A2; //counter clockwise joint
    int Len1; //arm segment 1
    int Len2; //arm segment 2
    double D1; //dividing angle A1 joint
    double D2; //dividing angle A1 joint
    double Dist; //distance between tip of segment 2 and root joint
    double TipX; //x coordinate of tip
    double TipY; //y coordinate of tip
};


#endif //KINEMATICS_ANGLES_H
