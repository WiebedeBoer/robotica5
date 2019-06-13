//
// Created by Wouter Nauta on 2019-06-06.
//Altered by Wiebe 2019-11-06.
//

#include "iostream"
#include "JoystickArm.h"
//#include "Node.h"

Node* JoystickArm::GetPathsVertical(int length, int height) {
	std::vector<Node> possibilitiesExact;
	std::vector<Node> possibilitiesOther;

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
						Node n2 = *node2;
						possibilitiesExact.push_back(n2);
					}
					else {
						Node n2 = *node2;
						possibilitiesOther.push_back(n2);
					}
				}
				delete node2;
			}
			delete node1;
		}
		delete node0;
	}

	//exact and other, exact pakkken, vervolgens beste optie selecteren

	std::vector<Node> bothVector;
	std::vector<Node> heightVector;
	std::vector<Node> lengthVector;

	if (!possibilitiesExact.size()) {
		for (Node nExact : possibilitiesExact) {

			if (nExact.GetHeight() == height && nExact.GetLength() == length) {
				// beide gelijk
				bothVector.push_back(nExact);
			}
			else if (nExact.GetHeight() == height) {
				heightVector.push_back(nExact);
			}
			else if (nExact.GetLength() == length) {
				lengthVector.push_back(nExact);
			}
		}

	}
	else if (!possibilitiesOther.size()) {
		for (Node nOther : possibilitiesOther)
		{
			if (nOther.GetHeight() == height && nOther.GetLength() == length) {
				// beide gelijk
				bothVector.push_back(nOther);
			}
			else if (nOther.GetHeight() == height) {
				heightVector.push_back(nOther);
			}
			else if (nOther.GetLength() == length) {
				lengthVector.push_back(nOther);
			}
		}

	}

	//other
	//eerst both vector, minste angle, anders heightvector
	//angle change, minste is
	//hoogte hetzelfde, lengte dichtbij het zelfde
	//en andersom. dus de lengte, dan dichtbij de hoogte

	//exact lengte exact hetzelfde anders zit het in other

	//stel gevuld
	//op basis prioriteit beste node
	//dat return

	/*
	//exact and other, exact pakkken, vervolgens beste optie selecteren
	//std::reverse(possibilitiesExact.begin(), possibilitiesExact.end());
	//std::reverse(possibilitiesOther.begin(), possibilitiesExact.end());
	for (Node* nExact : possibilitiesExact)
	{		
		//for (Node* nOther : possibilitiesOther)
		//{
			if (nExact->GetHeight() == nExact->GetHeight()) {
				//return nExact; //beste moves //hoogte en lengte prioriteit 1
				//hoogte gelijk 2
				//lengte glijk 3
			}
			//then length
			else {
				if (nExact->GetLength() == nExact->GetLength()) {
					//return nExact; //prioriteit 1
				}
				//else not then other
				else {
					//return nOther; //prioriteit 2 hoogte, lengte 3
				}				
			}
		//}
	}
	//return nExact;

	//minste angle verandering filteren
	return possibilitiesExact;

	for (Node* nOther : possibilitiesExact)
	{
		//for (Node* nOther : possibilitiesOther)
		//{
		if (nOther->GetHeight() == nOther->GetHeight()) {
			//return nExact;
		}
		//then length
		else {
			if (nOther->GetLength() == nOther->GetLength()) {
				//return nExact;
			}
			//else not then other
			else {
				//return nOther;
			}
		}
		//}
	}
	return possibilitiesOther;
	*/

	//eerst kijken naar de dingen bij de hoogte exact gelijk is, die van hoogte nemen
	//hoogte prioriteit
	//exact gelijk, dan die heeft die voorrang
	//gewoon recursief, geen tree search
	//wanneer node gereturned, dan angle beschikbaar .angle child . angle
	//3 nodes, eerste last = servo 4, dan servo 3, servo 2, servo 1
	//verticale 1 2 3	
	//servo 2, 3, 4 correct verticaal

	//verandering in angles beste move eruit
	//possibilities niet toegevoegd, wel ergens toegevoegd


	//return nullptr; 
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

int JoystickArm::NodeToPin(Node node) {
	//int angle
	Angles dlg; //instance of angles
	//int n = dlg.AngleToPin(angle, servo); //angle to pin for a particular servo
	//int n = dlg.AngleToPin(angle0, VerticalServos); //angle to pin for a particular servo

	//n.AngleToPin(angle, servo); //angle to pin for a particular servo
	//dlg.AngleToPin(nExact.angle0, VerticalServos); //angle to pin for a particular servo
	//Node n = node;

	//dlg.AngleToPin(0, 1);
	//int nodpin = dlg.AngleToPin(n.Angle, VerticalServos); //angle to pin for a particular servo
	int nodpin = dlg.AngleToPin(node.Angle, node.ServoArm.Id); //angle to pin for a particular servo

	return nodpin;
}