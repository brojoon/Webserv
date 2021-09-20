#ifndef SERVER_HPP
#define SERVER_HPP

class Location;

//#include "client.hpp"
#include "Location.hpp"
#include <map>


class Server
{
	private:
		std::string host;
		std::string server_name;
		std::string root;
		std::vector<unsigned short> ports;
		int server_fd;
		std::map<std::string, Location> locations;
		unsigned int client_max_body_size;
		std::vector<std::string> default_files;
		std::map<int, std::string> error_pages;

		//std::map<int, client> clients;

	public:
		Server();
		virtual ~Server();

		const std::string &getHost() const;
		const std::string &getServerName() const;
		std::vector<unsigned short> &getPorts();
		const int &getServer_fd() const;
		const std::string &getRoot() const;
		std::map<std::string, Location> &getLocations();
		const unsigned int &getClientMaxBodySize() const;
		std::vector<std::string> &getDifaultFiles();
		std::map<int, std::string> &getErrorPages();
		//std::map<int, client> &getClients();

		void setHost(const std::string &host);
		void setServerName(const std::string &server_name);
		void setfd(int server_fd);
		void setRoot(std::string &root);
		void setClientMaxBodySize(unsigned int client_max_body_size);

		bool init_Server(int &server_fd);
};

#endif