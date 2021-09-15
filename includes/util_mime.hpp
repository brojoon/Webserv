#ifndef UTIL_MIME_HPP
#define UTIL_MIME_HPP

#include <map>
#include <string>
#include <iostream>

namespace ft
{
	class mime
	{
		private:
			std::map<std::string, std::string> _mime;

		public:
			mime();
			~mime();
			std::string get_mime_type(std::string extension);
	};
}

#endif