#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>

class Location;

class Server
{
	private:
		std::string server_name;
		std::string ip;
		unsigned short port;
		int server_fd;
		std::map<std::string, Location> locations;

	public:
		Server();
		virtual ~Server();

		std::string getServerName() const;
		std::string getIp() const;
		unsigned short getPort() const;
		int getServer_fd() const;
		std::map<std::string, Location> &getLocations() const;

		void setServerName(const std::string &server_name);
		void setIp(const std::string &ip);
		void setPort(unsigned short &port);
		void setfd(int &server_fd);

		bool init_Server(int &server_fd);
};

#endif