#ifndef MSG_CHECKER_HPP
#define MSG_CHECKER_HPP

#include <iostream>
#include <map>
#include <string>
#include "utils.hpp"
#include "Webserver.hpp"
#include <map>
#include <unistd.h>

class msg_checker
{
	public:
		typedef struct return_type
		{
			std::string method;
			std::string url_abs_path;
			std::string query;
			std::string version;
			std::string status;
			std::string ip;
			std::string port;
			//요청
			std::vector<std::string> accept;
			std::vector<std::string> accept_Language;
			std::vector<std::string> accept_Encoding;
			std::string Use_Agent;
			//일반
			std::string Content_Language;
			std::string Content_Length;
			std::string Content_Location;
			std::string Content_Type;
			std::string Last_Modified;
		}return_type;

	private:
		return_type info;
	public:
		msg_checker();
		return_type check(std::string &firstline, std::map<std::string, std::string> &map);
		std::string	find_url();
		void 		find_redirect_url(std::string& root, std::string redirect_Uri);
		void 		check_indexfile(std::string& root);
};

#endif