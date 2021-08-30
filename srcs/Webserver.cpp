#include <iostream>
#include "Webserver.hpp"

Webserver *Webserver::instance;

Webserver::Webserver()
{

}

Webserver::~Webserver()
{
	delete instance;
}

Webserver *Webserver::getInstance()
{
	if (instance == NULL)
		instance = new Webserver;
	return (instance);
}

std::map<std::string, Server> &Webserver::getServerList()
{
	return this->server_list;
}

bool Webserver::parsing(char *str)
{
	--str;
	return true;

}

