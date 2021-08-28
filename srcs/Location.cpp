#include  "Location.hpp"

Location::Location()
{

}

Location::~Location()
{

}

const std::string &Location::getLocationName()
{
	return this->location_name;
}

std::vector<std::string> &Location::getAllowMethods()
{
	return this->allow_methods;
}

bool Location::getAutoIndex()
{
	return this->auto_index;
}

void Location::setLocationName(std::string &Location_name)
{
	this->Location_name = Location_name;
}

void Location::setAllowMethods(std::string &allow_methods)
{
	this->allow_methods = allow_methods;
}

void setAutoIndex(bool &autoindex)
{
	this->auto_index = autoindex;
}

