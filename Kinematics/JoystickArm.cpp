//
// Created by Wouter Nauta on 2019-06-06.
//Altered by Wiebe 2019-11-06.
//

#include "JoystickArm.h"

std::vector<std::tuple<Servo &, int>> JoystickArm::GetPaths() {
    std::vector<Node&> possibilities;
    for (int angle0 = VerticalServos[0].Angle - 30; angle0 < VerticalServos[0].Angle + 30; angle0++) {
        if (angle0 >= VerticalServos[0].MinRotation and angle0 <= VerticalServos[0].MaxRotation) {
            Node* node0 = new Node(angle0, VerticalServos[0]);
            for (int angle1 = VerticalServos[1].Angle - 30; angle1 < VerticalServos[1].Angle + 30; angle1++) {
                if (angle1 >= VerticalServos[1].MinRotation and angle1 <= VerticalServos[1].MaxRotation) {
                    Node* node1 = new Node(angle1, VerticalServos[1], node0);
                    for (int angle2 = VerticalServos[2].Angle - 30; angle2 < VerticalServos[2].Angle + 30; angle2++) {
                        if (angle2 >= VerticalServos[2].MinRotation and angle2 <= VerticalServos[2].MaxRotation) {
                            Node* node2 = new Node(angle2, VerticalServos[2], node1);
                            possibilities.push_back(node2)
                        }
                    }
                }
            }
        }
    }
}