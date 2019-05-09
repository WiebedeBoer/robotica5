#include "Move.h"
#include <iostream>

Move::Move(Arduino a, std::string t) : Command( a,t)
{
}

Move::~Move()
{
}
void Move::Execute()
{
	std::cout << "RobotMoves" << std::endl;
}
