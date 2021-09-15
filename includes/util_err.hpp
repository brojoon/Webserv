#ifndef UTIL_ERR_HPP
#define UTIL_ERR_HPP

#include <map>
#include <string>
#include <iostream>

namespace ft
{
	class err
	{
		private:
			std::map<std::string, std::string> _err;

		public:
			err();
			~err();
			std::string get_err(std::string status);
	};
}

#endif