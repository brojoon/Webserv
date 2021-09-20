#include "../includes/util_env.hpp"

namespace ft
{
	env::env(std::string length,
		std::string extention,
		std::string url,
		std::string query,
		std::string method,
		std::string host,
		std::string port,
		std::string version)
	{
		_vec_env[0] = "AUTH_TYPE=Basic";
		_vec_env[1] = "CONTENT_LENGTH=" + length;
		_vec_env[2] = "CONTENT_TYPE=" + ft::mime().get_mime_type(extention);
		_vec_env[3] = "GATEWAY_INTERFACE=CGI/1.1";
		_vec_env[4] = "PATH_INFO=." + url;
		_vec_env[5] = "PATH_TRANSLATED=." + url;
		_vec_env[6] = "QUERY_STRING=" + query;
		_vec_env[7] = "REMOTE_ADDR=127.0.0.1";
		_vec_env[8] = "REMOTE_IDENT=";
		_vec_env[9] = "REMOTE_USER=";
		_vec_env[10] = "REQUEST_METHOD=" + method;
		_vec_env[11] = "REQUEST_URI=." + url + "?" + query;
		_vec_env[12] = "SCRIPT_NAME=." + url;
		_vec_env[13] = "SERVER_NAME=" + host;
		_vec_env[14] = "SERVER_PORT=" + port;
		_vec_env[15] = "SERVER_PROTOCOL=HTTP/"+ version;
		_vec_env[16] = "SERVER_SOFTWARE=42Webserv/1.0";
	}

	env::~env() {}

	char **env::get_env()
	{
		for (int i = 0;  i < 17; i++)
			_env[i] = strdup(_vec_env[i].c_str());
		_env[17] = 0;
		return (_env);
	}
}