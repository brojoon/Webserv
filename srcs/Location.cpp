#include  "Location.hpp"

Location::Location()
{

}

Location::~Location()
{

}

const std::string &Location::getLocationPath() const
{
	return this->location_path;
}

const bool &Location::getAutoIndex() const
{
	return this->autoindex;
}

const std::string &Location::getRoot() const
{
	return this->root;
}

std::vector<std::string> &Location::getAllowMethods()
{
	return this->allow_methods;
}

void Location::setLocationPath(std::string &location_path)
{
	this->location_path = location_path;
}

void Location::setAutoIndex(bool autoindex)
{
	this->autoindex = autoindex;
}

void Location::setRoot(std::string &root)
{
	this->root = root;
}

