#ifndef MSG_HPP
#define MSG_HPP

#include <iostream>
#include <map>
#include <string>

class msg_checker
{
	public:
		typedef struct return_type
		{
			std::string status;
			std::string url_abs_path;
			// ...
		}return_type;
	public:
		msg_checker();
		return_type check(std::string &firstline, std::map<std::string, std::string> &map);
};

#endif