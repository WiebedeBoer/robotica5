#ifndef SOCKETCLASSLINUX_H
#define SOCKETCLASSLINUX_H

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
class SocketServer 
{
private:
	//helper function om string naar char array om te zetten vergeet niet om delete aante roepen als de char array gesloten moet worden. 
	char* convertstrtochar(std::string s);
	//wacht op antwoord van de client
	void waitMsg(char buf[]);
	//maak verbinding met de client
	bool makeConnection();
	int clientSocket;
public:
	SocketServer();
	//stuurt command naar de client en returnt het response
	std::string executeCommand(std::string cmd);

};
#endif // SOCKETCLASSLINUX_H
