//
// Created by Wouter Nauta on 2019-06-06.
//Altered by Wiebe 2019-11-06.
//

#include "JoystickArm.h"

Node* JoystickArm::GetPathsVertical(int length, int height) {
	std::vector<Node *> possibilitiesExact;
	std::vector<Node *> possibilitiesOther;
	for (int angle0 = VerticalServos[0].MinRotation; angle0 < VerticalServos[0].MaxRotation; angle0++) {
		Node *node0 = new Node(angle0, VerticalServos[0]);
		for (int angle1 = VerticalServos[1].MinRotation; angle1 < VerticalServos[1].MaxRotation + 30; angle1++) {
			Node *node1 = new Node(angle1, VerticalServos[1], node0);
			for (int angle2 = VerticalServos[2].MinRotation; angle2 < VerticalServos[2].MaxRotation + 30; angle2++) {
				Node* node2 = new Node(angle2, VerticalServos[2], node1);
				double node2Length = node2->GetLength();
				double node2Height = node2->GetHeight();
				if (round(node2Length) == length && round(node2Height) == height) {
					if (node2Length == length || node2Height == height) {
						possibilitiesExact.push_back(node2);
					}
					else {
						possibilitiesOther.push_back(node2);
					}
				}
				else {
					delete node2;
				}
			}
		}

	}
	return nullptr; //exact and other, exact pakkken, vervolgens beste optie selecteren
	//eerst kijken naar de dingen bij de hoogte exact gelijk is, die van hoogte nemen
	//hoogte prioriteit
	//exat gelijk, dan die heeft die voorrang
	//gewoon recursief, geen tree search
	//wanneer node gereturned, dan angle beschikbaar .angle child . angle
	//3 nodes, eerste last = servo 4, dan servo 3, servo 2, servo 1
	//verticale 1 2 3	
	//servo 2, 3, 4 correct verticaal

	//AngleToPin(angle, servo); //angle to pin for a particular servo
}