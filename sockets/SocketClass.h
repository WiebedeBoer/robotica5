#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")
class SocketServer 
{
private:
	char* convertstrtochar(std::string s);
	void waitMsg(char buf[]);
	bool makeConnection();
	SOCKET clientSocket;
public:
	SocketServer();
	std::string executeCommand(std::string cmd);
	void close();

};