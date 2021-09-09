#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "msg_checker.hpp"
#include "utils.hpp"
#include <sys/wait.h>

using namespace ft;


class client
{
	public:
		typedef msg_checker::return_type checker_return_type;

	private:
		int socker_num;
		//리퀘스트 메시지 파싱
		void parse_msg(std::string &request_msg);
		//리퀘스트 메시지 파싱해서 아래의 두 변수에 저장
		std::string _first_line;
		std::map<std::string, std::string> _header_field;
		//msg_checker 객체
		//msg_checker _obj;
		//msg_chekcer 객체가 점검을 끝내고 반환하는 값을 info에 저장
		checker_return_type _info;
		std::string cgi_process();
	public:
		client(std::string request_msg);
		client(int socket, int port);
		std::string get_response();
		std::string get_location_header();
		int getSockNum();
};

#endif
