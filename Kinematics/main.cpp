#include <iostream>
//#include "Node.h"
#include "Angles.h"
//#include "JoystickArm.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
	//JoystickArm dlg; //instance of joystickarm
	//Node *n = dlg.GetPathsVertical(1, 2);
	//Node n = JoystickArm::GetPathsVertical(1, 2);
	//std::cout << n->GetHeight() << std::endl;
	//std::cout << n->GetLength() << std::endl;
	double x = 5.0;
	double y = 5.0;
	//a1, a2 = angles(x, y);
	Angles dlg;
	//double a, double b = dlg.Gonio(x, y);
	std::cout << dlg.Gonio(x, y) << std::endl;
    return 0;
}