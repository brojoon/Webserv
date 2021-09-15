#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include "msg_checker.hpp"
#include "utils.hpp"

using namespace ft;


class client
{
	public:
		typedef msg_checker::return_type checker_return_type;

	private:
		int socket_num;
		bool chunk_error;
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
		std::string _autoindex();
		std::map<int, bool> flag;
		bool is_read_end;
		std::string chunk_check(std::string &src, int pos);
	public:
		client();
		std::string get_response();
		std::string get_body();
		void 		exe_method();
		void 		delet_file();
		int 		getSockNum();
		void		post_upload();
		void		request(int socket, int port);
		bool		isReadEnd();
		void		bodySizeError(std::map<int, std::string> &map, int pos, int socket, int port, std::string errnum);
};

#endif
