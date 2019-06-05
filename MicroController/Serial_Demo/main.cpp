#include <iostream>;
#include <wiringSerial.h>;
#include <string>
#include <poll.h>
//send string to serial interface 
void Send(std::string str, int fd) {
	str = str + '\n';
	char* chars = &str[0];
	//std::cout << "send: " << std::string(chars) << std::endl;
	serialPuts(fd, chars);
}
//returns the checksum from the given string
int CheckSum(std::string str) {
	int sum = 0;
	for (int i = 0; i < str.length(); i++) {
		sum += (int)str[i];
	}
	return sum;
}
//returns 1 when data is received
int WaitForData(int fd, int timeout)
{
	struct pollfd poll_struct = { fd, POLLIN, 0 };
	return poll(&poll_struct, 1, timeout);
}
//waits 1 second for a message from the serial interface
std::string WaitForMessage(int &fd) {

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
}
//returns true when ack belongs to the send
bool ackresponse(std::string ack, std::string send) {
	std::string delimiter = "|";
	ack = ack.substr(ack.find(delimiter) + 1);
	if (!ack.empty() && ack[ack.length() - 1] == '\n') {
		ack.erase(ack.length() - 1);
	}
	send = send.substr(0, send.find(delimiter));


	if (ack == send) {
		return true;
	}
	else {
		return false;

	}

}
//sends string to serial interface with retries until message has been ack
void SerialSend(std::string input) {

	int fd;
	std::string inputsum;
	fd = serialOpen("/dev/ttyACM0", 9600);
	std::cout << fd << std::endl;

	std::cin >> input;
	input = input + "|";
	inputsum = std::to_string(CheckSum(input));
	std::string sendstring = input + std::string(inputsum);
	do
	{
		try
		{
			Send(sendstring, fd);
		}
		catch (const std::exception &) {

			std::cout << "An Error occured" << std::endl;
		}

	} while (!ackresponse(WaitForMessage(fd), sendstring));

	serialClose(fd);
}
int main(int argc, char* argv[])
{
	std::string input;
	while (true)
	{
		std::cin >> input;
		SerialSend(input);
	}
}

