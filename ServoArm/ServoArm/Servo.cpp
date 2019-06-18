//
// Created by Wouter Nauta on 2019-06-06.
//
#include "pch.h"
#include "Servo.h"

Servo::Servo(int _id, int _minRotation, int _maxRotation, int _angle, int _length) : Id(_id), MinRotation(_minRotation), MaxRotation(_maxRotation), Angle(_angle), Length(_length) {}