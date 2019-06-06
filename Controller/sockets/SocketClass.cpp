#include "pch.h"
#include "SocketClass.h"
SocketServer::SocketServer()
{
	makeConnection();
}
void SocketServer::close() {
	// Close the socket
	closesocket(clientSocket);
	// Cleanup winsock
	WSACleanup();
}

bool SocketServer::makeConnection() {
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return false;
	}
	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		return false;
	}
	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1234);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 
	bind(listening, (sockaddr*)&hint, sizeof(hint));
	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);
	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on
	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	}
	// Close listening socket
	closesocket(listening);
	// While loop: accept and echo message back to client
	return true;
}

std::string SocketServer::executeCommand(std::string cmd) {
	char* confcmd = convertstrtochar(cmd);
	send(clientSocket, confcmd, cmd.length(), 0);
	delete[] confcmd;
	char buf[4096];
	//does it work?????????????
	waitMsg(buf);
	//cout << "final awnser = " << buf << '\n';
	return buf;
}
char* SocketServer::convertstrtochar(std::string s) {
	int i;
	const int x = s.length();
	char* p = new char[x];
	for (i = 0; i < s.length(); i++) {
		p[i] = s[i];
	}
	return p;
}
void SocketServer::waitMsg(char buf[]) {
	ZeroMemory(buf, 4096);

	int bytesReceived = recv(clientSocket, buf, 1024, 0);
	std::string msg2 = std::string(buf, 0, bytesReceived);
	if (bytesReceived == SOCKET_ERROR)
	{

	}
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
