#include "Arduino.h"

std::string Response;
//send string to serial interface 
void Arduino::Send(std::string str, int fd) {
	str = str + '\n';
	char* chars = &str[0];
	//std::cout << "send: " << std::string(chars) << std::endl;
	serialPuts(fd, chars);
}
//returns the checksum from the given string

int Arduino::CheckSum(std::string str) {
	int sum = 0;
	for (int i = 0; i < str.length(); i++) {
		sum += (int)str[i];
	}
	//Serial.println(sum);
	return sum;
}
//returns 1 when data is received
int Arduino::WaitForData(int fd, int timeout)
{
	struct pollfd poll_struct = { fd, POLLIN, 0 };
	return poll(&poll_struct, 1, timeout);
}
//waits 1 second for a message from the serial interface
std::string Arduino::WaitForMessage(int &fd) {

	uint8_t rx_byte = '9';

	bool MessageCompleted = false;
	std::string message;
	int Datareceived = WaitForData(fd, 1000);
	if (Datareceived == 1) {
		while (MessageCompleted == false) {
			int dataavail = serialDataAvail(fd);
			while (dataavail > 0) {

				rx_byte = serialGetchar(fd);

				if (MessageCompleted == false) {
					message += rx_byte;
				}

				if (rx_byte == '\n') {
					MessageCompleted = true;
					serialFlush(fd);
					return message;
				}
			}
		}
	}
	return "No message received";
}
//returns true when ack belongs to the send
bool Arduino::ackresponse(std::string ack, std::string send) {
	int acksum, calcsum;
	std::cout << ack << std::endl;
	if (!ack.empty() && ack[ack.length() - 1] == '\n') {
		
		std::regex rgx("(ack):(.*)<(.*)>\\|(\\d*)(.*)");
		std::smatch m;
		std::regex_search(ack, m, rgx);
		std::string Sumable = std::string(m[1].str() + ":" + m[2].str() +"<"+ m[3].str() + ">" + "|");
		acksum = CheckSum(Sumable);
		calcsum =  std::stoi(m[4]);
		Response = m[3].str();
		if (acksum != calcsum) {
			
			std::cout << "ChecksumError" << std::endl;
		}
		ack = (m[2].str() + "|" + m[4].str());
		send = send.substr(0,send.find("|")) +"|"+ std::to_string(calcsum);
	}
	if (ack == send) {
		std::cout << "Ack!" << std::endl;

		return true;
	}
	else if (ack == "No message received") {
		return false;
	}
	else {
		std::cout << "NAck!" << std::endl;
		return false;
	}

}
//sends string to serial interface with retries until message has been ack
void Arduino::SerialSend(std::string input) {

	int fd;
	std::string inputsum;
	fd = serialOpen(UsbPort, 9600);
	std::cout << fd << std::endl;
	input = input + "|";
	int NackCount = 0;
	inputsum = std::to_string(CheckSum(input));
	std::string sendstring = input + std::string(inputsum);
	do
	{
		try
		{
			NackCount++;
			Send(sendstring, fd	);
		}
		catch (const std::exception &) {

			std::cout << "An Error occured" << std::endl;
		}

	} while (!ackresponse(WaitForMessage(fd), sendstring) && NackCount != 5);
	if (NackCount == 5) {
		std::cout << "CommandDropped After 5 tries" << std::endl;
	}
	else {
		std::cout << "MessageSend" << std::endl;
	}
	serialClose(fd);
}

std::string Arduino::GetLastResponce(){
return Response;
}
Arduino::Arduino(char* usb)
{
	UsbPort = usb;
}


Arduino::Arduino()
{
}
Arduino::~Arduino()
{
}

