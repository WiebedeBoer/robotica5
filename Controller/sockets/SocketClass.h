#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")
class SocketServer 
{
private:
	//helper function om string naar char array om te zetten vergeet niet om delete aante roepen als de char array gesloten moet worden. 
	char* convertstrtochar(std::string s);
	//wacht op antwoord van de client
	void waitMsg(char buf[]);
	//maak verbinding met de client
	bool makeConnection();
	SOCKET clientSocket;
public:
	SocketServer();
	//stuurt command naar de client en returnt het response
	std::string executeCommand(std::string cmd);
	void close();

};