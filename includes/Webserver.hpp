#ifndef webserver_hpp
#define webserver_hpp

#include "../includes/Server.hpp"
#include "../includes/Location.hpp"

#define WEBSERVER Webserver::getInstance()
#define BUF_SIZE 102410
#define SMALL_BUF 100

#include <map>
#include <set>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <ctime>
#include <cmath>
#include <sys/select.h>

typedef struct ServerFD
{
	int socket;
	struct sockaddr_in serv_adr;

}ServerFD;

class Server;

class Webserver
{
	private:
		Webserver();
		static Webserver *instance;
		std::map<int, Server> server_list;
		std::set<unsigned short> listen_port;
		std::map<int, unsigned short> client_sockets; 
		std::map<int, ServerFD> server_sockets; 
		std::map<int, std::string> response_list;
		std::map<int, bool> is_socket_end;
		fd_set read_set, read_temp, write_set, write_temp;
	public:
		virtual ~Webserver();
		
		static Webserver* getInstance();
		std::map<int, Server> &getServerList();
		std::set<unsigned short> &getListenPort();
		std::map<int, unsigned short> &getClientSockets();
		std::map<int, ServerFD> &getServerSockets();
		std::map<int, bool> &getIsSocketEnd();
		fd_set &getReadSet();
		fd_set &getWriteSet();
		

		void removeOnSelect(int socket);
		bool parsingConfig(const char *config_file);
		void error_handling(const char *message);
		void initWebServer();
		std::map<int, std::string> write_map;
};


#endif