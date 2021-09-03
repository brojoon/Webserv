#include "../includes/msg_checker.hpp"

msg_checker::msg_checker()
{
	info.status = "200";
}

msg_checker::return_type msg_checker::check(std::string &firstline, std::map<std::string, std::string> &map)
{
 	info.method = ft::ft_strtok(firstline, " ");
	std::string path = ft::ft_strtok(firstline, " ");
	std::string http = ft::ft_strtok(firstline, "/");
	info.version = ft::ft_strtok(firstline, "/");

	info.url_abs_path = ft::ft_strtok(path, "?");
	info.query = path;

	if (ft::isknown(info.method) == false)
		info.status = "501";
	else if (ft::isMethods(info.method) == false)
		info.status = "405";
	
	////////////////////////////////////////////////////////////////////

	info.ip = ft::ft_strtok(map["HOST"], ":");
	info.port = map["HOST"];

	int i_port = atoi(info.port.c_str());
	if (WEBSERVER->getServerList().find(i_port) != (WEBSERVER->getServerList().end()))
	{
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
		info.status = "400";
	}
	
	return return_type();
}
