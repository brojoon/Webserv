#include <iostream>
#include "../includes/Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

const std::string &Server::getServerName() const
{
	return this->server_name;
}

const std::string &Server::getRoot() const
{
	return this->root;
}

const unsigned short &Server::getPort() const
{
	return this->port;
}

const int &Server::getServer_fd() const
{
	return this->server_fd;
}

std::map<std::string, Location> &Server::getLocations()
{
	return this->locations;
}


//std::map<int, client> &Server::getClients()
//{
//	return this->clients;
//}


void Server::setPort(unsigned short port)
{
	this->port = port;
}

void Server::setfd(int server_fd)
{
	this->server_fd = server_fd;
}

void Server::setServerName(const std::string &server_name)
{
	this->server_name = server_name;
}

void Server::setRoot(std::string &root)
{
	this->root = root;
}

bool Server::init_Server(int &server_fd)
{
	this->server_fd = server_fd;
	return true;
}


