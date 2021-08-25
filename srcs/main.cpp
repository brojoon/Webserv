#include "main.hpp"

int parsingConfig(std::string str)
{
	static_cast<void>(str);
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "ERROR usage: ./webserv configFile" << std::endl;
		return 1;
	}

	if (parsingConfig(argv[1]) == 0)
		return 1;

	return 0;
}