//
// Created by Wouter Nauta on 2019-06-06.
//Altered by Wiebe 2019-11-06.
//

#include "JoystickArm.h"

std::vector<std::tuple<Servo &, int>> JoystickArm::GetPaths() {
    std::vector<std::tuple<Servo &, int>> possibilities;
    for (int angle0 = VerticalServos[0].Angle - 30; angle0 < VerticalServos[0].Angle + 30; angle0++) {
        if (angle0 >= VerticalServos[0].MinRotation and angle0 <= VerticalServos[0].MaxRotation) {
            Node node(angle0, VerticalServos[0]);
            for (int i = VerticalServos[1].Angle - 30; i < VerticalServos[1].Angle + 30; i++) {
                if (i >= VerticalServos[1].MinRotation and i <= VerticalServos[1].MaxRotation) {

                    for (int i = VerticalServos[2].Angle - 30; i < VerticalServos[2].Angle + 30; i++) {
                        if (i >= VerticalServos[2].MinRotation and i <= VerticalServos[2].MaxRotation) {

                        }
                    }
                }
            }
        }
    }
}