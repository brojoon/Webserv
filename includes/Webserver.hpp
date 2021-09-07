#ifndef Webserver_HPP
#define Webserver_HPP

#include "../includes/Server.hpp"
#include "../includes/Location.hpp"

#define WEBSERVER Webserver::getInstance()

#include <map>
#include <set>

class Server;

class Webserver
{
	private:
		Webserver();
		static Webserver *instance;
		std::map<int, Server> server_list;
		std::set<unsigned short> listen;
	public:
		virtual ~Webserver();
		
		static Webserver* getInstance();
		std::map<int, Server> &getServerList();
		std::set<unsigned short> &getListen();

		bool parsingConfig(const char *config_file);

};


#endif