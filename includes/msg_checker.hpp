#ifndef MSG_CHECKER_HPP
#define MSG_CHECKER_HPP

#include <iostream>
#include <map>
#include <string>
#include "utils.hpp"
#include "Webserver.hpp"
#include <map>
#include <unistd.h>

class msg_checker
{
	public:
		typedef struct return_type
		{
			std::string method;
			std::string url_abs_path;
			std::string query;
			std::string version;
			std::string status;
			std::string ip;
			std::string port;
			// ...
		}return_type;
	private:
		return_type info;
	public:
		msg_checker();
		return_type check(std::string &firstline, std::map<std::string, std::string> &map);
};

#endif