#include "utils.hpp"

namespace ft
{
	void split(const std::string &src, const std::string& delimiter, std::vector<std::string> &dst)
	{
		std::string tmp;

		for (std::string::const_iterator iter = src.begin(); iter != src.end(); iter++)
		{
			if (sep.find(*iter) == std::string::npos )
				tmp += *iter;
			else
			{
				if (tmp != "")
				{
					dst.push_back(tmp);
					tmp.clear();
				}
			}
		}
		if (tmp != "")
			dst.push_back(tmp);
	}

	bool isMethods(const std::string &src)
	{
		if (src == "GET" || src == "POST" \
			src == "DELETE" || src == "PUT")
		{
			return true;
		}
		return false;
	}
}

