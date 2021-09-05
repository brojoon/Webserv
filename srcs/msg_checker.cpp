#include "../includes/msg_checker.hpp"
#include "../includes/Server.hpp"

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
	
	/*
	(void)firstline;
	(void)map;;
 	info.method = "GET";
	std::string path = "index.html?";
	std::string http = "HTTP";
	info.version = "1.1";
	*/

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
	/*
	if (WEBSERVER->getServerList().find(i_port) != (WEBSERVER->getServerList().end()))
	{
		info.ip = "127.0.0.1";
		info.port = "8080";
	}
	*/

	std::cout << "aaa" << std::endl;
	if (WEBSERVER->getServerList().find(i_port) != (WEBSERVER->getServerList().end()))
	{
		std::cout << "bbb" << std::endl;
		std::string root;
		std::string dir = info.url_abs_path;
		std::string sub_dir;
		if (info.url_abs_path[info.url_abs_path.size() - 1] != '/') // "/test/index.html" -> "/test" -> "/test/index.html/"
		{
			sub_dir = dir;
			int idx = dir.size() - 1;
			while (dir[idx--] != '/')
				dir.pop_back();
			dir.pop_back();
		}
		else
			dir.pop_back();
		if (WEBSERVER->getServerList()[i_port].getLocations().find(dir) \
						!= WEBSERVER->getServerList()[i_port].getLocations().end())
		{
			root = WEBSERVER->getServerList()[i_port].getLocations()[info.url_abs_path].getRoot();
		}
		else
		{
			root = WEBSERVER->getServerList()[i_port].getRoot();
		}
		info.url_abs_path = root + std::string("/") + info.url_abs_path;
	}
	else
	{
		std::cout << "ccc" << std::endl;
		info.status = "400";
	}
	std::cout << "ddd" << std::endl;
	return return_type();
}
