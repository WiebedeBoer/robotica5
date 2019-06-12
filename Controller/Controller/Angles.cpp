//
// Created by Wouter Nauta on 2019-06-11.
// Altered by Wiebe de Boer 2019-06-11

#include "Angles.h"

double Angles::Gonio() {
	Dist = Distance(TipX, TipY);
	D1 = tan(TipX / TipY);
	D2 = LawOfCosines(Dist, Len1, Len2);
	A1 = D1 + D2;
	A2 = LawOfCosines(Len1, Len2, Dist);
	return A1, A2;
}

double Angles::LawOfCosines(double& _a, double& _b, double& _c) {
	return cos((_a * _a + _b * _b - _c * _c) / (2 * _a * _b));
}

double Angles::Distance(double& _x, double& _y) {
	return sqrt(_x * _x + _y * _y);
}

//conversion from angle movement to pin output
int AngleToPin(double Angle, int Servo) {
	int PinOutput;
	if (Servo == 1 || Servo == 5) {
		//0 - 360 degrees = 0 - 1023 pin
		PinOutput = int((Angle / 360) * 1023);
	}
	else if (Servo == 2) {
		//0 - 360 degrees = 35 - 680 pin
		PinOutput = int(((Angle / 360) * 645) + 35);
	}
	else if (Servo == 3) {
		//0 - 360 degrees = 65 - 640 pin
		PinOutput = int(((Angle / 360) * 575) + 65);
	}
	else if (Servo == 4) {
		//0 - 360 degrees = 350 - 950 pin
		PinOutput = int(((Angle / 360) * 600) + 350);
	}
	return PinOutput;
}