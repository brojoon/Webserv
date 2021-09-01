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

////////////////////////////////////////////////////////////////////////////

struct paresed_request_msg
{
	std::string first_line;
	std::map<std::string, std::string> header_field;
};

class parse_request_msg_and_store_and_response
{
	private:
		struct paresed_request_msg _parsed_request_msg;
		//config file에서 필요한 정보 저장 -> config담당에서 적절한 객체를 만들어 필요한 정보를 저장해 둬야할듯(이 객체를 현재 클래스의 생성자 쪽에서 받는게 좋을듯)
		std::string _status;//error_check()함수에서 초기화해줄듯
		std::string _abs_path;
		//제너럴 헤더필드
		std::string get_date();
		std::string get_pragma();
		std::string get_cache_control();
		std::string get_connection();
		std::string get_transfer_encoding();
		std::string get_via();
		//리퀘스트 헤더필드
		//리스폰스 헤더필드
		//엔터티 헤더필드
		
	public:
		parse_request_msg_and_store_and_response(std::string request_msg);
		void error_check();//config file 참조해서 error_chceck한다. rfc에 맞지않는 부분이 있는지도 체크 -> 에러발생시 (throw 예외)
		std::string get_response();

};

#endif