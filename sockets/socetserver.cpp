// socetserver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "SocketClassLinux.h"
int main()
{
	SocketServer server = SocketServer();
	while(true){
		std::string cmd;
		std::cin >> cmd;
		std::cout << server.executeCommand(cmd) << '\n';
	}
	server.close();
}
