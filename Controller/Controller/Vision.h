#pragma once

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class Vision
{
public:
	Vision();
	~Vision();	
	std::string executeCommand(std::string cmd);	
private:
	bool MakeConnection();
	char* convertstrtochar(std::string s);
	void waitMsg(char buf[]);
	int clientSocket;
};

