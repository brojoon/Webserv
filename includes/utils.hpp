#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <cmath>
#include "../includes/util_env.hpp"
#include "../includes/util_mime.hpp"
#include "../includes/util_err.hpp"

namespace ft
{
	void config_print();
	void split(const std::string &src, const std::string& delimiter, std::vector<std::string> &dst);
	bool isMethods(const std::string &src);
	bool isknown(const std::string &src);
	std::string get_next_line(const std::string &src);
	std::string ft_strtok(std::string &src, std::string deli);
	int ft_contain(const std::string &src, std::string sub);
	const std::string currentDateTime();
	std::string getFileModifiedTime(const std::string &src) ;
	long hexaStringToLong(const std::string &str);
	std::string longToHexaString(long src);
	std::string::size_type find_first_of(const char *src, const char *find_in_str, std::string::size_type pos);
}
#endif