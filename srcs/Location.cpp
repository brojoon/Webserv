#include  "Location.hpp"

Location::Location()
{

}

Location::~Location()
{

}

const std::string &Location::getLocationPath()
{
	return this->location_path;
}

std::vector<std::string> &Location::getAllowMethods()
{
	return this->allow_methods;
}

bool Location::getAutoIndex()
{
	return this->auto_index;
}

std::string Location::getRoot()
{
	return this->root;
}

void Location::setLocationName(std::string &Location_path)
{
	this->Location_path = Location_path;
}

void Location::setAllowMethods(std::string &allow_methods)
{
	this->allow_methods = allow_methods;
}

void setAutoIndex(bool &autoindex)
{
	this->auto_index = autoindex;
}

void setRoot(std::string root)
{
	this->root = root;
}

