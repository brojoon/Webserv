#include "client.hpp"

void client::parse_msg(std::string &request_msg)
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

std::string client::get_next_line(const std::string &src)
{
	static int end = 0;
	// 변수 end std::string::npos 와 비교시 타입 size_type 으로 수정해야 합니다 컴파일 X !!

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

std::string client::ft_strtok(std::string &src, std::string deli)//인자로 들어온 src를 참조만 하는게 아니라, 편집도 함
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

bool client::ft_contain(const std::string &src, std::string sub)
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
