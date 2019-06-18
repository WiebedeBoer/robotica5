//
// Created by Wouter Nauta on 2019-06-07.
//

#include "Node.h"

Node::Node(int _angle, const Servo& _servo, Node* _last) : ServoArm(_servo), Last(_last) {
	Angle = _angle;
}
Node::Node(int _angle, const Servo& _servo) : ServoArm(_servo), Last(nullptr) {
	Angle = _angle;
}

double Node::GetLength() {
	double length = cos(Angle) * ServoArm.Length;
	if (Last == nullptr)
	{
		return length;
	}
	else {
		return length + Last->GetLength();
	}
}

double Node::GetHeight() {
	double height = sin(Angle) * ServoArm.Length;
	if (Last == nullptr)
	{
		return height;
	}
	else {
		return height + Last->GetHeight();
	}
}
int Node::AngleChanges() {
	double change = abs(ServoArm.Angle - Angle);

	if (Last == nullptr) {
		return change;
	}
	else {
		return change + Last->AngleChanges();
	}
}