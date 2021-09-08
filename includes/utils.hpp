#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <cmath>

namespace ft
{
	void config_print();
	void split(const std::string &src, const std::string& delimiter, std::vector<std::string> &dst);
	bool isMethods(const std::string &src);
	bool isknown(const std::string &src);
	std::string get_next_line(const std::string &src);
	std::string ft_strtok(std::string &src, std::string deli);
	bool ft_contain(const std::string &src, std::string sub);
	const std::string currentDateTime();
	std::string getFileModifiedTime(const std::string &src) ;
	long hexaStringToLong(const std::string &str);
	std::string longToHexaString(long src);


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