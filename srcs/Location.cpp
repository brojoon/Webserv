#include  "../includes/Location.hpp"

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

std::vector<std::string> &Location::getLimitExcept()
{
	return this->limit_except;
}

const std::string &Location::getRedirection() const
{
	return this->redirection;
}

std::vector<std::string> &Location::getDifaultFiles()
{
	return this->default_files;
}

std::map<std::string, std::string> &Location::getCgiPath()
{
	return this->cgi_path;
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

void Location::setRedirection(std::string &redirection)
{
	this->redirection = redirection;
}