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
			 || src == "DELETE" || src == "PUT")
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
	
	env::env(std::string length,
		std::string extention,
		std::string url,
		std::string query,
		std::string method,
		std::string host,
		std::string port,
		std::string version)
	{
		_vec_env[0] = "AUTH_TYPE=Basic";
		_vec_env[1] = "CONTENT_LENGTH=" + length;
		_vec_env[2] = "CONTENT_TYPE=" + ft::mime().get_mime_type(extention);
		_vec_env[3] = "GATEWAY_INTERFACE=CGI/1.1";
		_vec_env[4] = "PATH_INFO=." + url;
		_vec_env[5] = "PATH_TRANSLATED=." + url;
		_vec_env[6] = "QUERY_STRING=" + query;
		_vec_env[7] = "REMOTE_ADDR=127.0.0.1";
		_vec_env[8] = "REMOTE_IDENT=";
		_vec_env[9] = "REMOTE_USER=";
		_vec_env[10] = "REQUEST_METHOD=" + method;
		_vec_env[11] = "REQUEST_URI=." + url + "?" + query;
		_vec_env[12] = "SCRIPT_NAME=." + url;
		_vec_env[13] = "SERVER_NAME=" + host;
		_vec_env[14] = "SERVER_PORT=" + port;
		_vec_env[15] = "SERVER_PROTOCOL=HTTP/"+ version;
		_vec_env[16] = "SERVER_SOFTWARE=42Webserv/1.0";
	}

	env::~env() {}

	char **env::get_env()
	{
		for (int i = 0;  i < 17; i++)
			_env[i] = strdup(_vec_env[i].c_str());
		_env[17] = 0;
		return (_env);
	}

	mime::mime()
	{
		_mime[".aac"] = "audio/aac";
		_mime[".abw"] = "application/x-abiword";
		_mime[".arc"] = "application/octet-stream";
		_mime[".atom"] = "application/atom+xml";
		_mime[".avi"] = "video/x-msvideo";
		_mime[".azw"] = "application/vnd.amazon.ebook";
		_mime[".bin"] = "application/octet-stream";
		_mime[".bmp"] = "image/x-ms-bmp";
		_mime[".bz"] = "application/x-bzip";
		_mime[".bz2"] = "application/x-bzip2";
		_mime[".csh"] = "application/x-csh";
		_mime[".css"] = "text/css";
		_mime[".csv"] = "text/csv";
		_mime[".doc"] = "application/msword";
		_mime[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
		_mime[".epub"] = "application/epub+zip";
		_mime[".gif"] = "image/gif";
		_mime[".htm"] = "text/html";
		_mime[".html"] = "text/html";
		_mime[".ico"] = "image/x-icon";
		_mime[".ics"] = "text/calendar";
		_mime[".jar"] = "application/java-archive";
		_mime[".jpeg"] = "image/jpeg";
		_mime[".jpg"] = "image/jpeg";
		_mime[".js"] = "application/js";
		_mime[".json"] = "application/json";
		_mime[".mid"] = "audio/midi";
		_mime[".midi"] = "audio/midi";
		_mime[".mov"] = "video/quicktime";
		_mime[".mpeg"] = "video/mpeg";
		_mime[".mpkg"] = "application/vnd.apple.installer+xml";
		_mime[".mp3"] = "audio/mpeg";
		_mime[".mp4"] = "video/mp4";
		_mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
		_mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
		_mime[".odt"] = "application/vnd.oasis.opendocument.text";
		_mime[".oga"] = "audio/ogg";
		_mime[".ogg"] = "audio/ogg";
		_mime[".ogv"] = "video/ogg";	
		_mime[".ogx"] = "application/ogg";
		_mime[".pdf"] = "application/pdf";
		_mime[".php"] = "text/html";
		_mime[".png"] = "image/png";
		_mime[".ppt"] = "application/vnd.ms-powerpoint";
		_mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
		_mime[".rar"] = "application/x-rar-compressed";
		_mime[".rtf"] = "application/rtf";
		_mime[".sh"] = "application/x-sh";
		_mime[".svg"] = "image/svg+xml";
		_mime[".svgz"] = "image/svg+xml";
		_mime[".swf"] = "application/x-shockwave-flash";
		_mime[".tar"] = "application/x-tar";
		_mime[".tif"] = "image/tiff";
		_mime[".tiff"] = "image/tiff";
		_mime[".ttf"] = "application/x-font-ttf";
		_mime[".txt"] = "text/plain";
		_mime[".vsd"] = "application/vnd.visio";
		_mime[".war"] = "application/java-archive";
		_mime[".wav"] = "audio/x-wav";
		_mime[".wave"] = "audio/wave";
		_mime[".weba"] = "audio/webm";
		_mime[".webm"] = "video/webm";
		_mime[".webp"] = "image/webp";
		_mime[".woff"] = "font/woff";
		_mime[".woff2"] = "font/woff2";
		_mime[".wmv"] = "video/x-ms-wmv";
		_mime[".xhtml"] = "application/xhtml+xml";
		_mime[".xls"] = "application/vnd.ms-excel";
		_mime[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
		_mime[".xml"] = "application/xml";
		_mime[".xul"] = "xul";
		_mime[".zip"] = "application/zip";
		_mime[".3gpp"] = "video/3gpp";
		_mime[".3gp"] = "video/3gpp";
		_mime[".3g2"] = "video/3gpp2";
		_mime[".7z"] = "application/x-7z-compressed";	
	}
	mime::~mime(){}
	std::string mime::get_mime_type(std::string extension)
	{
		if (_mime.count(extension))
			return _mime[extension];
		return ("application/octet-stream");
	}

	err::err()
	{
		_err["100"] = "Continue";
		_err["101"] = "Switching protocols";

		_err["200"] = "OK";
		_err["201"] = "Created";
		_err["202"] = "Accepted";
		_err["203"] = "Non-authoritative information";
		_err["204"] = "No content";

		_err["301"] = "Moved permanently";
		_err["302"] = "Not temporarily";
		_err["304"] = "Not modified";

		_err["400"] = "Bad Request";
		_err["401"] = "Unauthorized";
		_err["402"] = "Payment required";
		_err["403"] = "Forbidden";
		_err["404"] = "Not found";
		_err["405"] = "Method not allowed";
		_err["407"] = "Porxy authentication required";
		_err["408"] = "Request timeout";
		_err["410"] = "Gone";
		_err["412"] = "Precondition failed";
		_err["414"] = "Request-URI too long";

		_err["500"] = "Internal server error";
		_err["501"] = "Not implemented";
		_err["503"] = "Service unnailable";
		_err["504"] = "Gateway timeout";
		_err["505"] = "HTTP version not supported";
	}
	err::~err(){}
	std::string err::get_err(std::string status)
	{
		if (_err.count(status))
			return _err[status];
		return ("");
	}
}


