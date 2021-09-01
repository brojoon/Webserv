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

#include "utils_for_parse.hpp"

struct paresed_request_msg
{
	std::string first_line;
	std::map<std::string, std::string> header_field;
};

class parse_request_msg_and_store_and_response
{
	private:
		//리퀘스트 메시지 파싱해서 _parsed_request_msg변수에 저장
		struct paresed_request_msg _parsed_request_msg;
		//'리스폰스 메시지 관련 함수들'에서 사용될 변수들
		std::string _status;
		std::string _abs_path;

	private:
		//리퀘스트 메시지 관련 함수
		void method_check(const std::string &str);
		void url_check(const std::string &str);
		void http_check(const std::string &str);
		void version_check(const std::string &str);
		void header_field_check(const std::string &str);
		//리스폰스 메시지 관련 함수(get_response함수에서 사용될 예정)
		std::string get_reponse_line();
		std::string get_header_field();

	public:
		parse_request_msg_and_store_and_response(std::string request_msg);
		void error_check(); //config file 참조해서 error_chceck한다. rfc에 맞지않는 부분이 있는지도 체크 -> 에러발생시 (throw 예외)
		std::string get_response();
};

#endif