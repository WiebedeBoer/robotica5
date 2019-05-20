#include "Vision.h"



Vision::Vision()
{
	while (!MakeConnection()) {
		MakeConnection();
	}
}
bool Vision::MakeConnection() {
	// Create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		return false;
	}
	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1234);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);
	// Wait for a connection
	// Wait for a connection
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];      // Client's remote name
	char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

	memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	}
	// Close listening socket
	close(listening);
	// While loop: accept and echo message back to client
	return true;
}
std::string Vision::executeCommand(std::string cmd) {
	char* confcmd = convertstrtochar(cmd);
	send(clientSocket, confcmd, cmd.length(), 0);
	delete[] confcmd;
	char buf[4096];
	//does it work?????????????
	waitMsg(buf);
	std::cout << "final awnser = " << buf << '\n';
	return buf;
}
char* Vision::convertstrtochar(std::string s) {
	int i;
	const int x = s.length();
	char* p = new char[x];
	for (i = 0; i < s.length(); i++) {
		p[i] = s[i];
	}
	return p;
}
void Vision::waitMsg(char buf[]) {
	memset(buf, 0,4096);

	int bytesReceived = recv(clientSocket, buf, 1024, 0);
	std::string msg2 = std::string(buf, 0, bytesReceived);
	
	if (msg2 == "Ask") {
		//send stuff from controler
		send(clientSocket, "kip", 3, 0);
		//cout << string(buf, 0, bytesReceived) << endl;
		waitMsg(buf);

	}
	else {
	}
	//cout << string(buf, 0, bytesReceived) << endl;
}
Vision::~Vision()
{
}
