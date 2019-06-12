//
// Created by Wouter Nauta on 2019-06-06.
//Altered by Wiebe 2019-11-06.
//

#include "iostream"
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

	//exact and other, exact pakkken, vervolgens beste optie selecteren
	std::reverse(possibilitiesExact.begin(), possibilitiesExact.end());
	std::reverse(possibilitiesOther.begin(), possibilitiesExact.end());
	for each (Node* nExact in possibilitiesExact)
	{		
		for each (Node* nOther in possibilitiesOther)
		{
			if (nExact->GetHeight() == nOther->GetHeight()) {
				return nExact;
			}
			//then length
			else {
				if (nExact->GetLength() == nOther->GetLength()) {
					return nExact;
				}
				//else not then other
				else {
					return nOther;
				}				
			}
		}
	}




	//eerst kijken naar de dingen bij de hoogte exact gelijk is, die van hoogte nemen
	//hoogte prioriteit
	//exat gelijk, dan die heeft die voorrang
	//gewoon recursief, geen tree search
	//wanneer node gereturned, dan angle beschikbaar .angle child . angle
	//3 nodes, eerste last = servo 4, dan servo 3, servo 2, servo 1
	//verticale 1 2 3	
	//servo 2, 3, 4 correct verticaal


	return nullptr; 
	//exact and other, exact pakkken, vervolgens beste optie selecteren
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

int NodeToPin() {
	//int angle
//Angles dlg; //instance of angles
//int n = dlg.AngleToPin(angle, servo); //angle to pin for a particular servo
//int n = dlg.AngleToPin(angle0, VerticalServos); //angle to pin for a particular servo

//n.AngleToPin(angle, servo); //angle to pin for a particular servo
//dlg.AngleToPin(nExact.angle0, VerticalServos); //angle to pin for a particular servo
//nExact nod;

//dlg.AngleToPin(0, 1);
//int nodpin = dlg.AngleToPin(0, 1); //angle to pin for a particular servo

//return nodpin;
}