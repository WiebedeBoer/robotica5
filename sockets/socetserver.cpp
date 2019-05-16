// socetserver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "SocketClass.h"
#pragma comment (lib, "ws2_32.lib")
int main()
{
	SocketServer server = SocketServer();
	while(true){
		server.executeCommand("do something productive");
	}
	server.close();
}
