#include <iostream>
#include "../includes/Server.hpp"

Server::Server() : server_fd(-1), client_max_body_size(0)
{

}

Server::~Server()
{

}

const std::string &Server::getHost() const
{
	return this->host;
}

const std::string &Server::getServerName() const
{
	return this->server_name;
}

const std::string &Server::getRoot() const
{
	return this->root;
}

std::vector<unsigned short> &Server::getPorts()
{
	return this->ports;
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

const unsigned int &Server::getClientMaxBodySize() const
{
	return this->client_max_body_size;
}

void Server::setHost(const std::string &host)
{
	this->host = host;
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

void Server::setClientMaxBodySize(unsigned int client_max_body_size)
{
	this->client_max_body_size = client_max_body_size;
}

bool Server::init_Server(int &server_fd)
{
	this->server_fd = server_fd;
	return true;
}


