#ifndef UTIL_ENV_HPP
#define UTIL_ENV_HPP

#include <map>
#include <string>
#include <iostream>
#include "../includes/util_mime.hpp"

namespace ft
{
	class env
	{
		private:
			std::map<int, std::string> _vec_env;
			char *_env[18];

		public:
			env(std::string length,
				std::string extention,
				std::string url,
				std::string query,
				std::string method,
				std::string host,
				std::string port,
				std::string version);
			~env();
			char **get_env();		
	};
}

#endif