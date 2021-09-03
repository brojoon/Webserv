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
	_info = _obj.check(_first_line, _header_field);
}

client::client(int socket)
{
	int ret, bufsize = 2560;
	char buf[bufsize];
	std::string str = "[aaaaaa]\r\n\r\n";

	while (1)
	{	
		//ret = read(socket, buf, bufsize - 1);
		//buf[ret] = 0;
		//str += std::string(buf);
		if (ft_contain(str, "\r\n\r\n"))// 헤더의 끝
		{
			parse_msg(str);
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

