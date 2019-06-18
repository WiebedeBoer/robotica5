//
// Created by Wouter Nauta on 2019-06-11.
// Altered by Wiebe de Boer 2019-06-11
#include <string>
#include <iostream>
#include <math.h>
#include "Angles.h"
const double pi = atan(1) * 4;
std::vector<int> Angles::Gonio(double TipX, double TipY) {

	Dist = Distance(TipX, TipY);

	if (Dist > 0 && Dist <= 346) {
		D1 = atan(TipY / TipX) * 180 / pi;
		D2 = LawOfCosines(Dist, Len1, Len2) * 180 / pi;
		A1 = D1 + D2;
		A2 = LawOfCosines(Len1, Len2, Dist) * 180 / pi;

		AnglePinOutput1 = AngleToPin(A1, 2);
		AnglePinOutput2 = AngleToPin(A2, 3);

		std::vector<int> AnglePinOut;
		AnglePinOut.insert(AnglePinOut.begin(), AnglePinOutput1);
		AnglePinOut.insert(AnglePinOut.begin(), AnglePinOutput2);

		compensate = Compensator(90 - D2, TipX, A2);

		AnglePinOutput3 = AngleToPin(compensate, 4);
		AnglePinOut.insert(AnglePinOut.begin(), AnglePinOutput3);
		return AnglePinOut;
	}
	else {
		return std::vector<int>();
	}

}

double Angles::LawOfCosines(double& _a, double& _b, double& _c) {
	double cosD = acos((_a * _a + _b * _b - _c * _c) / (2 * _a * _b));
	return cosD;
}

double Angles::Distance(double& _x, double& _y) {
	double sqrtD = sqrt(_x * _x + _y * _y);
	return sqrtD;
}

//conversion from angle movement to pin output
int Angles::AngleToPin(double Angle, int Servo) {
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

double Angles::Compensator(double compensatingAlpha, double lTotal, double A2) {
	//int CompensatorOutput = compensating;
	//double CompensatorOutput = asin(compensating / Len1);
	double lA = sin(compensatingAlpha) * Len1;
	double lB = lTotal - lA;
	double CompensatorOutput = tan(A2) * lB;
	return CompensatorOutput;
}
