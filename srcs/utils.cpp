#include "../includes/utils.hpp"
#include "../includes/Webserver.hpp"
#include <cstring>

namespace ft
{
	void config_print()
	{	
		int i = 0;
		for (std::set<unsigned short>::iterator it = WEBSERVER->getListenPort().begin();
			it != WEBSERVER->getListenPort().end(); it++)
		{
			std::cout << "listen : " << *it << " ";
		}
		std::cout << std::endl;
		for  (std::map<int, Server>::iterator it = WEBSERVER->getServerList().begin();
			 it != WEBSERVER->getServerList().end(); it++, i++)
		{
			int j = 0;
			std::cout << "========== SERVER " << i << "==========" << std::endl;
			std::cout << "host: " << it->second.getHost() << std::endl;
			std::cout << "server_name: " << it->second.getServerName() << std::endl;
			std::cout << "server_root: " << it->second.getRoot() << std::endl;
			std::cout << "default_files: ";
			for (std::vector<std::string>::iterator it1 = it->second.getDifaultFiles().begin();
				it1 != it->second.getDifaultFiles().end(); it1++)
			{
				std::cout << *it1 << " ";
			}
			std::cout << std::endl;
			for (std::map<int, std::string>::iterator it1 = it->second.getErrorPages().begin();
				it1 != it->second.getErrorPages().end(); it1++)
			{
				std::cout << "error_page : " << it1->first << " " << it1->second << std::endl;
			}
			std::cout << "ports: ";
			for (std::vector<unsigned short>::iterator it1 = it->second.getPorts().begin();
				it1 != it->second.getPorts().end(); it1++)
			{
				std::cout << *it1 << " ";
			}
			std::cout << std::endl;
			std::cout << "server_fd: " << it->second.getServer_fd() << std::endl;
			std::cout << "client_max_body_size: " << it->second.getClientMaxBodySize() << std::endl;
			for (std::map<std::string, Location>::iterator it2 = it->second.getLocations().begin();
				it2 != it->second.getLocations().end(); it2++, j++)
			{
				std::cout << "========== Location " << j << "==========" << std::endl;
				std::cout << "Location_path: " << it2->second.getLocationPath() << std::endl;
				std::cout << "Location_root: " << it2->second.getRoot() << std::endl;
				std::cout << "Auto_Index: " << it2->second.getAutoIndex() << std::endl;
				std::cout << "limit_except: ";
				for (std::vector<std::string>::iterator it3 = it2->second.getLimitExcept().begin();
					it3 != it2->second.getLimitExcept().end(); it3++)
				{
					std::cout << *it3 << " ";
				}
				std::cout << std::endl;
				std::cout << "redirection: " << it2->second.getRedirection() << std::endl;
				std::cout << "default_files: ";
				for (std::vector<std::string>::iterator it4 = it2->second.getDifaultFiles().begin();
					it4 != it2->second.getDifaultFiles().end(); it4++)
				{
					std::cout << *it4 << " ";
				}
				std::cout << std::endl;
				std::cout << "cgi_except: ";
				for (std::vector<std::string>::iterator it = it2->second.getCigExcept().begin();
					it != it2->second.getCigExcept().end(); it++)
				{
					std::cout << *it << " ";
				}
				std::cout << std::endl;
				std::cout << "cgi_path: " << it2->second.getCgiPath() << std::endl;
			}
		}
	}

	void split(const std::string &src, const std::string &delimiter, std::vector<std::string> &dst)
	{
		std::string tmp;

		for (std::string::const_iterator iter = src.begin(); iter != src.end(); iter++)
		{
			if (delimiter.find(*iter) == std::string::npos )
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
			 || src == "DELETE")
		{
			return true;
		}
		return false;
	}

	bool isknown(const std::string &src)
	{
		if (src == "OPTIONS" || src =="GET"  || src =="HEAD"  || src =="POST" \
			|| src =="PUT" || src =="DELETE"  || src =="TRACE"  || src =="CONNECT")
		{
			return true;
		}
		return false;
	}

	std::string::size_type find_first_of(const char *src, const char *find_in_str, std::string::size_type pos)
	{
		int i;
		if (src == NULL || find_in_str == NULL)
			return std::string::npos;
		while (src[pos] != '\0')
		{
			i = 0;

			while (find_in_str[i] != '\0')
			{
				if (find_in_str[i] == src[pos])
					return pos;
				i++;
			}
			pos++;
		}
		return std::string::npos;
	}

	std::string get_next_line(const std::string &src)
	{
		static std::string::size_type end = 0;

		const int _size = src.size();
		std::string::size_type start = ((end == 0) ? 0 : end + 2);
		std::string ret;

		if (end == std::string::npos)
		{
			end = 0;
			ret = "\n\n";
			return ret;
		}
		end = find_first_of(src.c_str(), "\r", start);
		if (end == std::string::npos)//last line
			ret = src.substr(start, _size - start);
		else
			ret = src.substr(start, end -  start);//개행문자 빼고 substr
		return ret;
	}

	std::string ft_strtok(std::string &src, std::string deli)//인자로 들어온 src를 참조만 하는게 아니라, 편집도 함
	{
		int start = 0;
		std::string::size_type end = find_first_of(src.c_str(), deli.c_str(), 0);
		std::string ret;
		if (end == std::string::npos)
		{
			ret = src;
			src.clear();
		}
		else//aaa?   end = 3 start = 0
		{
			ret = src.substr(start, (end-1) -  start + 1);
			end = src.find_first_not_of(deli, end);
			if (end == std::string::npos)
				src.clear();
			else
				src = src.substr(end, src.size() - end);
		}
		return ret;//빈문자열이 return 될때가 종료조건
	}

	int ft_contain(const std::string &src, std::string sub)
	{
		int len = sub.size();
		int size =  src.size();
		for (int i = 0; i <= size - len; i++)
		{
			if (src.substr(i, len) == sub)
			{
				return i + len;
			}		
				
		}
		return -1;
	}

	// 현재시간을 string type으로 return하는 함수
	const std::string currentDateTime() {
		time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		if (tstruct.tm_hour >= 9)
			tstruct.tm_hour -= 9;
		else
		{
			tstruct.tm_hour += (24 - 9);
			tstruct.tm_mday -= 1;
		}
		strftime(buf, sizeof(buf), "%a, %d %b %Y %T GMT", &tstruct);
		return buf;
	}

	//파일의 마지막 수정시간을 string typedmfh return하는 함수
	std::string getFileModifiedTime(const std::string &src) 
	{
		struct stat attr;
		stat(src.c_str(), &attr);
		return std::string(ctime(&attr.st_mtime));
	}

	//"fff"와 같은 16진수 형식의 문자열을 숫자로 바꿔줌
	long hexaStringToLong(const std::string &str)
	{
		long ret = 0;
		std::map<char, int> map;
		int size = str.size();
		int temp;
		for (int i = 0; i < size; i++)
		{
			if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F'))
			{
				if (str[i] < 'a')
					temp = str[i] - '0';
				else
					temp = str[i] - 'a' + 10;
				ret += temp * pow(16, size - i -1);
			}
			else
				break;
		}
		return ret;
	}

	//16진수의 숫자를 16진수 형식의 문자열로 바꿔줌
	std::string longToHexaString(long src)
	{
		std::string ret;
		char ch;
		while (src != 0)
		{
			ch = src % 16;
			if (ch > 10)
				ch += 'a' - 10;
			else
				ch += '0';
			ret.insert(ret.begin(), ch);
			src /= 16;
		}
		return ret;
	}
}


