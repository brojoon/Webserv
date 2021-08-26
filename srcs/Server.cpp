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

std::string Server::getIp()
{
	return this->ip;
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

void Server::setIp(const std::string &ip)
{
	this->ip = ip;
}

void Server::setPort(unsigned short &port)
{
	this->port = port;
}

void Server::setfd(int &server_fd)
{
	this->server_fd = server_fd;
}

void setServerName(const std::string &server_name)
{
	this->server_name = server_name;
}

bool Server::init_Server(int &server_fd)
{
	this->server_fd = server_fd;
	return false;
}

