//
// Created by Wouter Nauta on 2019-06-07.
//

#ifndef KINEMATICS_NODE_H
#define KINEMATICS_NODE_H

#include <vector>
class Node {
public:
    std::vector<Node> prev;
    Node(){

    }
};


#endif //KINEMATICS_NODE_H
