#include <iostream>;
#include <wiringSerial.h>;
#include <string>

void Send(std::string str, int fd) {
	str = str +'\n';
	char* chars = &str[0];
	std::cout << "send: " << std::string(chars) << std::endl;
	serialPuts(fd, chars);
}
void Receive(int fd) {
	std::string result;
	//std::cout << serialDataAvail(fd) << std::endl;
	while (serialDataAvail(fd) != 0) {
		result += serialGetchar(fd);
	}
	std::cout << result << std::endl;
}
int CheckSum(std::string str) {
	int sum = 0;
	for (int i = 0; i < str.length(); i++) {
		sum += (int)str[i];
	}
	return sum;
}
int main(int argc, char* argv[])
{
	int fd;
	std::string input;
	std::string inputsum;
	fd = serialOpen("/dev/ttyACM0", 9600);
	std::cout << fd << std::endl;

	while (true)
	{
		std::cin >> input;
		input = input + "|";
		inputsum = std::to_string(CheckSum(input));
		std::string sendstring = input  + std::string(inputsum) ;
		Send(sendstring, fd);
		Receive(fd);

		std::cin.get();
	}

}

