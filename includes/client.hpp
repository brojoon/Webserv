#ifndef _PARSE_
#define _PARSE_

#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>

#include "msg_checker.hpp"
#include "utils_for_parse.hpp"

class client
{
	public:
		typedef typename msg_checker::return_type checker_return_type;
		
	private:
		//리퀘스트 메시지 파싱해서 _parsed_request_msg변수에 저장
		std::string first_line;
		std::map<std::string, std::string> header_field;
		//msg_checker 객체
		msg_checker obj;
		//msg_chekcer 객체가 점검을 끝내고 반환하는 값을 info에 저장
		checker_return_type info;

	public:
		client(std::string request_msg);
		std::string get_response();
};

#endif