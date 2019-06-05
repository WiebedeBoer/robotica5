#include <stdio.h>

#include "SocketClassLinux.h"
int main()
{
	std::cout << "start \n";
	SocketServer server = SocketServer();
	while(true){
		std::string cmd;
		std::cin >> cmd;
		std::cout << " RESULT = " << server.executeCommand(cmd) << '\n';
	}
}
