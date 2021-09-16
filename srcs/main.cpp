#include "../includes/client.hpp"
#include "../includes/utils.hpp"
#include "../includes/Webserver.hpp"
#include <signal.h>

void	handleSignal(int signo)
{
	if (signo == SIGINT || SIGKILL)
	{
		delete WEBSERVER;
		//std::cout << std::endl;
		exit(signo);
	}
	else
		return;
}

void set_signal(void)
{
	signal(SIGKILL, handleSignal);
	signal(SIGINT, handleSignal);
	signal(SIGPIPE, handleSignal);
}

int main(int argc, char *argv[])
{

	// if (argc != 2)
	// {
	// 	//std::cout << "ERROR usage: ./webserv configFile" << std::endl;
	// 	return 1;
	// }
	if (WEBSERVER->parsingConfig("./config/webserv.conf") == false)
		return 1;
	(void)argc;
	(void)argv;

	//ft::config_print();
	set_signal();
	WEBSERVER->initWebServer();
	delete WEBSERVER;

	return 0;
}