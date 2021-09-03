#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>

namespace ft
{
	void split(const std::string &src, const std::string& delimiter, std::vector<std::string> &dst);
	bool isMethods(const std::string &src);
	bool isknown(const std::string &src);
	std::string get_next_line(const std::string &src);
	std::string ft_strtok(std::string &src, std::string deli);
	bool ft_contain(const std::string &src, std::string sub);
}

#endif