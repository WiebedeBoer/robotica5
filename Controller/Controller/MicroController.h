#pragma once

#include <iostream>
#include <wiringSerial.h>
#include <string>
#include <poll.h>
#include <regex>
class MicroController
{
public:
	MicroController();
	MicroController(char* usb);
	~MicroController();
	void SerialSend(std::string input);
	std::string GetLastResponce();
	char* UsbPort;

private:
	void Send(std::string str, int fd);
	int CheckSum(std::string str);
	int WaitForData(int fd, int timeout);
	std::string WaitForMessage(int &fd);
	bool ackresponse(std::string ack, std::string send);
	int fd = 0;

};

