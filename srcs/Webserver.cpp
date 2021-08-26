#include "Webserver.hpp"
#include <iostream>

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

bool Webserver::parsing(char *str)
{
	--str;
	return true;

}

