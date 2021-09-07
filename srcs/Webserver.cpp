#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <set>
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

std::set<unsigned short> &Webserver::getListen()
{
	return this->listen;
}

bool Webserver::parsingConfig(const char *config_file)
{
	std::ifstream ifs;
	std::vector<std::string> chunked_config;
	std::string line;
	std::string location_path;
	std::string tmpstr;
	int left_curly_brace;
	int right_curly_brace;
	int index;
	int port;
	bool is_http;
	
	index = -1;
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
			ft::split(line, " \t;=", chunked_config);
			if (line.find('{') != std::string::npos)
				left_curly_brace++;
			if (line.find('}') != std::string::npos)
				right_curly_brace++;
			line.clear();
			if (ifs.eof())
				break ;
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
			if (*iter == "server")
			{
				index++;
				location_path.clear();
			}
			if (*iter == "listen")
			{
				iter++;
				if ((*iter).find(":") == std::string::npos)
					throw "ERROR : host and port must be configured";
				port = atoi((*iter).substr((*iter).find(":") + 1).c_str());
				tmpstr = (*iter).substr(0, (*iter).find(":"));
				if (tmpstr != "localhost" && tmpstr != "127.0.0.1")
					throw "ERROR : host is wrong";
				instance->server_list[index].getPorts().push_back((unsigned short)port);
				instance->server_list[index].setHost("localhost");
				instance->listen.insert((unsigned short)port);
				iter++;
				while (*iter == "listen")
				{
					iter++;
					if ((*iter).find(":") == std::string::npos)
						throw "ERROR : host and port must be configured";
					port = atoi((*iter).substr((*iter).find(":") + 1).c_str());
					tmpstr = (*iter).substr(0, (*iter).find(":"));
					for (std::vector<unsigned short>::iterator it = instance->server_list[index].getPorts().begin();
						it != instance->server_list[index].getPorts().end(); it++)
					{
						if ((unsigned short)port == *it)
							throw "ERROR : one server listen on the same port";
					}
					if (tmpstr != "localhost" && tmpstr != "127.0.0.1")
						throw "ERROR : host is wrong";
					instance->server_list[index].getPorts().push_back((unsigned short)port);
					instance->listen.insert((unsigned short)port);
					iter++;
				}
				
			}
			if (*iter == "server_name")
			{
				iter++;
				instance->server_list[index].setServerName(*iter);
				iter++;
			}
			else if (*iter == "client_max_body_size")
			{
				iter++;
				int tmp = atoi((*iter).c_str());
				if (tmp <= 0)
					throw "ERROR : client_max_body_size is wrong";
				instance->server_list[index].setClientMaxBodySize(static_cast<unsigned int>(tmp));
			}
			else if (*iter == "root")
			{
				iter++;
				if (location_path == "")
					instance->server_list[index].setRoot(*iter);
				else
					instance->server_list[index].getLocations()[location_path].setRoot(*iter);
			}
			else if (*iter == "error_page")
			{
				iter++;
				int key = atoi((*iter).c_str());
				iter++;
				instance->server_list[index].getErrorPages()[key] = *iter; 
			}
			else if (*iter == "location")
			{
				iter++;
				if ((*iter)[0] != '/')
					throw "ERROR : location path is wrong";
				location_path = *iter;
				instance->server_list[index].getLocations()[location_path].setLocationPath(location_path);
			}
			else if (*iter == "autoindex")
			{
				iter++;
				if (*iter == "off")
					instance->server_list[index].getLocations()[location_path].setAutoIndex(false);
				else if (*iter == "on")
					instance->server_list[index].getLocations()[location_path].setAutoIndex(true);
				else
					throw "ERROR : autoindex is wrong";
			}
			else if (*iter == "limit_except")
			{
				iter++;
				while (ft::isMethods(*iter) && iter != chunked_config.end())
				{
					instance->server_list[index].getLocations()[location_path].getLimitExcept().push_back(*iter);
					iter++;
				}
				if (iter == chunked_config.end())
					break;
				iter--;
			}
			else if (*iter == "return")
			{
				iter++;
				iter++;
				instance->server_list[index].getLocations()[location_path].setRedirection(*iter);
			}
			else if (*iter == "index")
			{
				iter++;
				while ((*iter).find('.') != std::string::npos && iter != chunked_config.end())
				{
					if (location_path == "")
						instance->server_list[index].getDifaultFiles().push_back(*iter);
					else
						instance->server_list[index].getLocations()[location_path].getDifaultFiles().push_back(*iter);
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

