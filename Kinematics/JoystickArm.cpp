//
// Created by Wouter Nauta on 2019-06-06.
//Altered by Wiebe 2019-11-06.
//

#include "JoystickArm.h"
#include <math.h>

std::vector<std::tuple<Servo&, int>> JoystickArm::GetPaths() {
	std::vector<std::tuple<Servo&, int>> possibilities;
	int index = 0;
	for (Servo s : VerticalServos) {
		for (int i = s.Angle - 30; i < s.Angle + 30; i++) {
			if (i >= s.MIN_ROTATION() and i <= s.MAX_ROTATION()) {
				std::tuple<Servo&, int> path = std::make_tuple(std::ref(s), i);
				possibilities.push_back(path);
			}
		}
		index++;
	}
	return possibilities;
}
