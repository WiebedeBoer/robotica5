//
// Created by Wouter Nauta on 2019-06-11.
//

#include "Angles.h"

double Angles::Gonio() {
    Dist = Distance(TipX, TipY);
    D1 = tan(TipX / TipY);
    D2 = LawOfCosines(Dist, Len1, Len2);
    A1 = D1 + D2;
	A2 = lawOfCosines(len1, len2, dist);
    return A1, A2;
}

double Angles::LawOfCosines(double _a, double _b, double _c) {
    return cos((_a * _a + _b * _b - _c * _c) / (2 * _a * _b));
}

double Angles::Distance(double _x, double _y) {
    return sqrt(_x * _x + _y * _y);
}