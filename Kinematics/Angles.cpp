//
// Created by Wouter Nauta on 2019-06-11.
//

#include "Angles.h"

double Angles::Gonio() {
    Dist = Distance(TipX, TipY);
    D1 = tan(TipX / TipY);
    D2 = LawOfCosines(Dist, Len1, Len2);
    A1 = D1 + D2;
    return A1, A2;
}

double Angles::LawOfCosines(double a, double b, double c) {
    return cos((a * a + b * b - c * c) / (2 * a * b));
}

double Angles::Distance(double x, double y) {
    return sqrt(x * x + y * y);
}