#ifndef Webserver_HPP
#define Webserver_HPP

#define WEBSERVER Webserver::getInstance()

#include <map>

class Server;

class Webserver
{
	private:
		Webserver();
		static Webserver *instance;
		std::map<int, Server> server_list;
	public:
		virtual ~Webserver();
		
		static Webserver* getInstance();
		std::map<int, Server> &getServerList();

		bool parsingConfig(const char *config_file);

};


#endif