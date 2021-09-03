#include "../includes/msg_checker.hpp"

msg_checker::msg_checker()
{
	info.status = "200";
}

msg_checker::return_type msg_checker::check(std::string &firstline, std::map<std::string, std::string> &map)
{
	(void)firstline;
	(void)map;;
 	info.method = "GET";
	std::string path = "index.html?";
	std::string http = "HTTP";
	info.version = "1.1";

	info.url_abs_path = ft::ft_strtok(path, "?");
	info.query = path;

	if (ft::isknown(info.method) == false)
		info.status = "501";
	else if (ft::isMethods(info.method) == false)
		info.status = "405";
	
	////////////////////////////////////////////////////////////////////

	info.ip = "127.0.0.1";
	info.port = "8080";

	int i_port = atoi(info.port.c_str());
	std::cout << "aaa" << std::endl;
	if (WEBSERVER->getServerList().find(i_port) != (WEBSERVER->getServerList().end()))
	{
		std::cout << "bbb" << std::endl;
		std::string root;
		if (WEBSERVER->getServerList()[i_port].getLocations().find(info.url_abs_path) \
						!= WEBSERVER->getServerList()[i_port].getLocations().end())
		{
			root = WEBSERVER->getServerList()[i_port].getLocations()[info.url_abs_path].getRoot();
		}
		else
		{
			root = WEBSERVER->getServerList()[i_port].getRoot();
		}
		info.url_abs_path = root + info.url_abs_path;
	}
	else
	{
		std::cout << "ccc" << std::endl;
		info.status = "400";
	}
	std::cout << "ddd" << std::endl;
	return return_type();
}
