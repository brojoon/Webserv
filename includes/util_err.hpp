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
			std::string _err403;
			std::string _err404;
		public:
			err();
			~err();
			std::string get403();
			std::string get404();
			std::string get_err(std::string status);
	};
}

#endif