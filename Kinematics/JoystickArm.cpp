//
// Created by Wouter Nauta on 2019-06-06.
//

#include "JoystickArm.h"
#include <math>

std::vector<std::tuple<Servo&, int>> JoystickArm::GetPaths() {
    std::vector<std::tuple<Servo&, int>> possibilities;
    int index = 0;
    for(Servo s : VerticalServos){
        for (int i = s.Angle - 30; i < s.Angle + 30; i++) {
            if(i >= s.MIN_ROTATION() and i <= s.MAX_ROTATION()){
                std::tuple<Servo&, int> path = std::make_tuple(std::ref(s), i);
                possibilities.push_back(path);
            }
        }
        index++;
    }
    return possibilities;
}

class Angles {
public:
	void Gonio();
private:
	int A1; //root joint
	int A2; //counter clockwise joint
	int len1; //arm segment 1
	int len2; //arm segment 2
	int D1; //dividing angle A1 joint
	int D2; //dividing angle A1 joint
	int dist; //distance between tip of segment 2 and root joint
	int tipx; //x coordinate of tip
	int tipy; //y coordinate of tip
};

void Gonio() {
	dist = distance(tipx, tipy);
	D1 = tan(tipx / tipy);
	D2 = lawOfCosines(dist, len1, len2);
	A1 = D1 + D2;
	
	return A1, A2;
}

void lawOfCosines(a, b, c) {
	return math.Acos((a*a + b * b - c * c) / (2 * a * b));
}

void distance(x, y) {
	return math.Sqrt(x*x + y * y);
}
