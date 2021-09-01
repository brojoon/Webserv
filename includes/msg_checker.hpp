#ifndef MSG_HPP
#define MSG_HPP

#include <iostream>
#include "client.hpp"

class msg_checker
{
	public:
		struct return_type
		{
			std::string status;
			std::string url_abs_path;
			// ...
		};

	public:
		msg_checker();
		return_type check(std::string &firstline, std::map<std::string, std::string> &map);
};

#endif