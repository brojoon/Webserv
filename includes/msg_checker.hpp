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
			//요청
			std::string method;
			std::string url_abs_path;
			std::string query;
			std::string version;
			std::string status;
			std::string host;
			int			port;
			std::string cgi_path;
			std::string extention;
			bool		is_cgi;
			std::map<int, std::string> error_pages;
			std::string location_uri;
			bool		same_location;
			std::string body;
			int			body_size;
			std::string body_type;
			std::string body_filename;

			bool		post_err;
			bool		check_autoindex;

			unsigned int max_body_size;
			bool		autoindex;

			std::vector<std::string> accept;
			std::vector<std::string> accept_Language;
			std::vector<std::string> accept_Encoding;
			std::string user_Agent;
		}return_type;

	private:
		return_type info;
	
	public:
		msg_checker();
		~msg_checker();
		return_type check(std::string &firstline, std::map<std::string, std::string> &map, int port);
		std::string	find_url(Server& server);
		void 		find_redirect_url(Server& server, std::string& root, std::string redirect_Uri);
		void		check_indexfile(std::string& root, std::vector<std::string> v_index);
		void		pase_body_for_post(std::map<std::string, std::string> &map);
};

#endif
