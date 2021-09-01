#include "utils_for_parse.hpp"

std::string get_next_line(const std::string &src)
{
	static int end = 0;
	const int _size = src.size();
	int start = ((end == 0) ? 0 : end + 1);
	std::string ret;

	if (end == std::string::npos)
	{
		end = 0;
		ret = "\n\n";
		return ret;
	}
	end = src.find_first_of("\r\n", start);
	if (end == std::string::npos)//last line
		ret = src.substr(start, _size - start);
	else
		ret = src.substr(start, (end-1) -  start + 1);//개행문자 빼고 substr
	return ret;
}

std::string ft_strtok(std::string &src, std::string deli)//인자로 들어온 src를 참조만 하는게 아니라, 편집도 함
{
	int start = 0;
	int end = src.find_first_of(deli);
	std::string ret;
	if (end == std::string::npos)
	{
		ret = src;
		src.clear();
	}
	else
	{
		ret = src.substr(start, (end-1) -  start + 1);
		end = src.find_first_not_of(deli, end);
		src = src.substr(end, src.size() - end);
	}
	return ret;//빈문자열이 return 될때가 종료조건
}

bool ft_contain(const std::string &src, std::string sub)
{
	int len = sub.size();
	int size =  src.size();
	for (int i = 0; i <= size - len; i++)
	{
		if (src.substr(i, len) == sub)
			return true;
	}
	return false;
}