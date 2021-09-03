#ifndef MSG_HPP
#define MSG_HPP

#include <iostream>
#include <map>
#include <string>
#include "../includes/utils.hpp"

class msg_checker
{
	public:
		typedef struct return_type
		{
			std::string method;
			std::string url_abs_path;
			std::string version;
			std::string status;
			// ...
		}return_type;
	private:
		return_type info;
	public:
		msg_checker();
		return_type check(std::string &firstline, std::map<std::string, std::string> &map);
};

#endif