#include "main.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "ERROR usage: ./webserv configFile" << std::endl;
		return 1;
	}
	
	if (WEBSERVER->parsingConfig(argv[1]) == false)
		return 1;

	return 0;
}