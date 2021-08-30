#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>

class Location;

class Server
{
	private:
		std::string server_name;
		std::string root;
		unsigned short port;
		int server_fd;
		std::map<std::string, Location> locations;

	public:
		Server();
		virtual ~Server();

		std::string getServerName() const;
		unsigned short getPort() const;
		int getServer_fd() const;
		std::map<std::string, Location> &getLocations();
		std::string getRoot();

		void setServerName(const std::string &server_name);
		void setPort(unsigned short &port);
		void setfd(int &server_fd);
		void setRoot(std::string &root);

		bool init_Server(int &server_fd);
};

#endif