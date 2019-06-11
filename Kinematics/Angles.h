//
// Created by Wouter Nauta on 2019-06-11.
//

#ifndef KINEMATICS_ANGLES_H
#define KINEMATICS_ANGLES_H


class Angles {
public:
    double Gonio();
    double LawOfCosines();
    double Distance();
private:
    int A1; //root joint
    int A2; //counter clockwise joint
    int Len1; //arm segment 1
    int Len2; //arm segment 2
    int D1; //dividing angle A1 joint
    int D2; //dividing angle A1 joint
    int Dist; //distance between tip of segment 2 and root joint
    int TipX; //x coordinate of tip
    int TipY; //y coordinate of tip
};


#endif //KINEMATICS_ANGLES_H
