#include <iostream>
#include "Node.h"
#include "JoystickArm.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
	JoystickArm dlg; //instance of joystickarm
	Node* n = dlg.GetPathsVertical(1, 2);
	//Node n = JoystickArm::GetPathsVertical(1, 2);
	//std::cout n.GetHeight << std::endl;
	//std::cout << n.GetLength << std::endl;
    return 0;
}