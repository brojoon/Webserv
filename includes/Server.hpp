#ifndef SERVER_HPP
#define SERVER_HPP

class Location;

#include "client.hpp"
#include "Location.hpp"
#include <map>


class Server
{
	private:
		std::string server_name;
		std::string root;
		unsigned short port;
		int server_fd;
		std::map<std::string, Location> locations;
		std::map<int, client> clients;

	public:
		Server();
		virtual ~Server();

		const std::string &getServerName() const;
		const unsigned short &getPort() const;
		const int &getServer_fd() const;
		const std::string &getRoot() const;
		std::map<std::string, Location> &getLocations();
		std::map<int, client> &getClients();

		void setServerName(const std::string &server_name);
		void setPort(unsigned short port);
		void setfd(int server_fd);
		void setRoot(std::string &root);

		bool init_Server(int &server_fd);
};

#endif