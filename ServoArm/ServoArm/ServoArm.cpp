// ServoArm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Node.h"
#include "JoystickArm.h"
int main() {
	std::cout << "Hello, World!" << std::endl;
	JoystickArm dlg; //instance of joystickarm
	Node n = dlg.GetPathsVertical(1, 2);
	//Node n = dlg.GetPathsVertical(1, 2);
	std::cout << n.GetHeight << std::endl;
	//std::cout << n.GetLength << std::endl;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
