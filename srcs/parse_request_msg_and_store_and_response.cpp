#include "parse_request_msg_and_store_and_response.hpp"


parse_request_msg_and_store_and_response::parse_request_msg_and_store_and_response(std::string request_msg)
{
	std::string str;
	std::string temp;
	std::string key;
	std::string value;

	//parse_request_msg_and_store
	str = get_next_line(request_msg);
	std::cout << str << std::endl;

	_parsed_request_msg.first_line = str;
	while ((str = get_next_line(request_msg)) != "\n\n")
	{
		key = ft_strtok(str, " :");
		value = str;
		_parsed_request_msg.header_field[key] = value;
	}
}

void parse_request_msg_and_store_and_response::error_check()//error_check함수에서 config에 대한 정보를 필요로함
{
	//request line error check
	std::string first_line = _parsed_request_msg.first_line;
	std::string temp;
	temp = ft_strtok(first_line, " ");
	//method_check(temp);
	temp = ft_strtok(first_line, " ");
	//url_check(temp);
	temp = ft_strtok(first_line, " ");
 	//http_check(temp);
	temp = ft_strtok(first_line, " ");
	//version_check(temp);

	std::map<std::string, std::string> header_field = _parsed_request_msg.header_field;
	//header_field_check();
}

std::string parse_request_msg_and_store_and_response::get_response()
{
	std::string ret;
	_status = std::string("200");
	ret += std::string("HTTP/1.1 ") + _status + std::string("OK\r\n");
	ret += std::string("Content-type: text/html; charset=UTF-8\r\n");// charset=UTF-8 이 부분 없으면 안됨(웹페이지가 불안정하게 표시될 수 있음)
	ret += std::string("\r\n");
	_abs_path = std::string("/Users/choihunjin/huchoi-git/study_web/instance/webserve/index.html");
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

void parse_request_msg_and_store_and_response::method_check(const std::string &str)
{
	if (str == "GET" || str == "POST" || str == "DELETE")
		return ;
	else
		throw std::runtime_error("method error 405"); // 예외번호가 더 있는데 일단 405만
}

void parse_request_msg_and_store_and_response::url_check(const std::string &str)
{

}

void parse_request_msg_and_store_and_response::http_check(const std::string &str)
{

}

void parse_request_msg_and_store_and_response::version_check(const std::string &str)
{

}
