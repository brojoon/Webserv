#include "../includes/client.hpp"

void client::parse_msg(std::string &request_msg)
{
	std::string str;
	std::string temp;
	std::string key;
	std::string value;

	str = get_next_line(request_msg);
	_first_line = str;
	while ((str = get_next_line(request_msg)) != "\n\n")
	{
		key = ft_strtok(str, " :");
		value = str;
		_header_field[key] = value;
	}
}

client::client(int socket)
{
	int ret, bufsize = 2560;
	char buf[bufsize];
	std::string str;

	while (1)
	{	
		ret = read(socket, buf, bufsize - 1);
		buf[ret] = 0;
		str += std::string(buf);
		if (ft_contain(str, "\r\n\r\n"))// 헤더의 끝
		{
			parse_msg(str);
			std::string content;
			if (_header_field.find("Content-Length") != _header_field.end())
			{
				int length = atoi(_header_field["Content-Lentgth"].c_str());
				ret = 0;
				while (length > 0)
				{
					ret = read(socket, buf, bufsize);
					length -= ret;
					buf[ret] = 0;
					content += std::string(buf);
				}
			}
			else if (_header_field.find("Transfer-Encoding") != _header_field.end())
			{
				std::string temp;
				
				unsigned int chunk;
				while (1)
				{
					while (1)
					{
						ret = read(socket, buf, 1);
						buf[ret] = 0;
						temp += std::string(buf);
						if (temp.find("\r\n") != std::string::npos)
						{
							chunk = atoi(temp.c_str());
							break;
						}
						temp.clear();
					}
					if (chunk == 0)
						break;
					while (chunk > 0)
					{
						ret = read(socket, buf, bufsize);
						buf[ret] = 0;
						content += std::string(buf);
						chunk -= ret;
					}
				}
			}
			_info = _obj.check(_first_line, _header_field);//content를 check()함수에 넘겨주어야함
			break;
		}
		else
			std::cout << ret << std::endl;
	}
}

/*
client::client(std::string request_msg)
{
	std::string str;
	std::string temp;
	std::string key;
	std::string value;

	str = get_next_line(request_msg);
	std::cout << str << std::endl;
	_first_line = str;
	while ((str = get_next_line(request_msg)) != "\n\n")
	{
		key = ft_strtok(str, " :");
		value = str;
		_header_field[key] = value;
	}
	_info = _obj.check(_first_line, _header_field);
}
*/

std::string client::get_response()
{
	std::string ret;
	std::string _status = std::string("200");
	ret += std::string("HTTP/1.1 ") + _status + std::string("OK\r\n");
	ret += std::string("Content-type: text/html; charset=UTF-8\r\n");// charset=UTF-8 이 부분 없으면 안됨(웹페이지가 불안정하게 표시될 수 있음)
	ret += std::string("\r\n");
	std::string _abs_path = std::string("/Users/choihunjin/huchoi-git/study_web/instance/webserve/index.html");
	int fd = open(_abs_path.c_str(), O_RDONLY);
	char buf[1000];
	int read_size;
	while ((read_size = read(fd, buf, 999)) != 0)
	{
		buf[read_size] = 0;
		ret += std::string(buf);
	}
	close(fd);
	return ret;
}

