#include <iostream>
#include "Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

std::string Server::getServerName()
{
	return this->server_name;
}

unsigned short Server::getPort()
{
	return this->port;
}

int Server::getServer_fd()
{
	return this->server_fd;
}

std::map<std::string, Location> &Server::getLocations()
{
	return this->locations;
}

std::string Server::getRoot()
{
	return this->root;
}

void Server::setPort(unsigned short &port)
{
	this->port = port;
}

void Server::setfd(int &server_fd)
{
	this->server_fd = server_fd;
}

void server::setServerName(const std::string &server_name)
{
	this->server_name = server_name;
}

void server::setRoot(std::string &root)
{
	this->root = root;
}

bool Server::init_Server(int &server_fd)
{
	this->server_fd = server_fd;
	return false;
}


