#ifndef Webserver_HPP
#define Webserver_HPP

#include <map>

#define WEBSERVER Webserver::getInstance()

class Server;

class Webserver
{
	private:
		Webserver();

		static Webserver *instance;
		std::map<std::string, Server> server_list;
	public:
		virtual ~Webserver();
		
		static Webserver* getInstance();
		std::map<std::string, Server> &getServerList();

		bool parsing(char *str);
};


#endif