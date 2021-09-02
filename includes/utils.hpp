#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

namespace ft
{
	void split(const std::string &src, const std::string& delimiter, std::vector<std::string> &dst);
	bool isMethods(const std::string &src);

}

#endif