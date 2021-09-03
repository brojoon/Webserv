#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "../includes/Webserver.hpp"
#include "../includes/utils.hpp"
#include "../includes/Server.hpp"


Webserver *Webserver::instance;

Webserver::Webserver()
{

}

Webserver::~Webserver()
{
	delete instance;
}

Webserver *Webserver::getInstance()
{
	if (instance == NULL)
		instance = new Webserver;
	return (instance);
}

std::map<int, Server> &Webserver::getServerList()
{
	return this->server_list;
}

bool Webserver::parsingConfig(const char *config_file)
{
	std::ifstream ifs;
	std::vector<std::string> chunked_config;
	std::string line;
	std::string location_path;
	int left_curly_brace;
	int right_curly_brace;
	int port;
	bool is_http;
	
	
	left_curly_brace = 0;
	right_curly_brace = 0;
	is_http = false;
	ifs.open(config_file);

	if (ifs.fail())
	{
		std::cerr << "Error : config_file open fail" << std::endl;
		return false;
	}
	try
	{
		while (1)
		{
			std::getline(ifs, line);
			if (ifs.eof())
				break ;
			ft::split(line, " \t;", chunked_config);
			if (line.find('{') != std::string::npos)
				left_curly_brace++;
			if (line.find('}' != std::string::npos))
				right_curly_brace++;
			line.clear();
		}
		if (left_curly_brace != right_curly_brace)
			throw "ERROR: curly_brace does not match";

		for (std::vector<std::string>::iterator iter = chunked_config.begin(); iter != chunked_config.end(); iter++)
		{
			if (*iter == "http")
			{
				if (is_http)
					throw "ERROR: config_file should have only one http block";
				is_http = true;

			}
			if (*iter == "server_name")
			{
				iter++;
				std::string server_name = *iter;
				iter++;
				iter++;
				port = atoi((*iter).c_str());
				if (instance->server_list.find(port) != instance->server_list.end())
				{
					if (instance->server_list[port].getServerName() == server_name)
						throw "ERROR : server_name and port_number must be different";
				}
				instance->server_list[port].setServerName(server_name);
				instance->server_list[port].setPort((unsigned short)port);
			}
			else if (*iter == "root")
			{
				iter++;
				if (location_path == "")
					instance->server_list[port].setRoot(*iter);
				else
					instance->server_list[port].getLocations()[location_path].setRoot(*iter);
			}
			else if (*iter == "location")
			{
				iter++;
				if ((*iter)[0] != '/')
					throw "ERROR : location path is wrong";
				location_path = *iter;
				instance->server_list[port].getLocations()[location_path].setLocationPath(location_path);
			}
			else if (*iter == "autoindex")
			{
				iter++;
				if (*iter == "off")
					instance->server_list[port].getLocations()[location_path].setAutoIndex(false);
				else if (*iter == "on")
					instance->server_list[port].getLocations()[location_path].setAutoIndex(true);
				else
					throw "ERROR : autoindex is wrong";
			}
			else if (*iter == "allow_methods")
			{
				iter++;
				while (ft::isMethods(*iter) && iter != chunked_config.end())
				{
					instance->server_list[port].getLocations()[location_path].getAllowMethods().push_back(*iter);
					iter++;
				}
				if (iter == chunked_config.end())
					break;
				iter--;
			}
		}
	}
	catch(const char *e)
	{
		std::cerr << e << std::endl;
		ifs.close();
		return false;
	}
	ifs.close();
	return true;
}

