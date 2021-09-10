#include "../includes/client.hpp"
#include "../includes/Webserver.hpp"
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>

std::string client::_autoindex()
{
	DIR *dir;
    struct dirent *ent;
	std::string t = _info.location_uri;
	std::string t2;
	t2.assign(t, t.find_last_of("/"), t.size() - t.find_last_of("/"));
	//t2.assign(t, 0, t.find_last_of("/") + 1);
	//std::string path = "./";
    std::string path = "." + t;
	std::cout << t2 << std::endl;
	dir = opendir (path.c_str());
	char buf[1000];
	struct stat state;
	std::string temp;
	std::string ret;
	getcwd(buf,999);
    if (dir != NULL) 
	{
		while ((ent = readdir (dir)) != NULL)
		{//std::string(localhost:) + std::to_string(_info.port)
			temp += path + std::string(ent->d_name);
			stat(temp.c_str(), &state);
			if (S_ISREG(state.st_mode))
			{
				ret += "<h2><a href=";
				ret += t2  + ent->d_name ;
				ret += std::string(">  ") + ent->d_name + "</a></h2>";
			}
			else if (S_ISDIR(state.st_mode))
			{
				ret += "<h1><a href=";
				if (ent->d_name == std::string("."))
					ret +=  t2 ;
				else if (ent->d_name == std::string(".."))
				{
					std::string test = t2;
					test.pop_back();
					std::string test2;
					test2.assign(test, 0, test.find_last_of("/") + 1);
					ret += test2;
				}
				else
					ret += t2  + ent->d_name + "/";
				ret += std::string(">  ") + ent->d_name + "/" + "</a></h1>";
			}
			temp.clear();
		}
		//std::cout << ret << std::endl;
		closedir (dir);
    } 
	else 
        perror ("");
	return ret;
}

void client::parse_msg(std::string &request_msg)
{
	std::string str;
	std::string temp;
	std::string key;
	std::string value;

	str = get_next_line(request_msg);
	_first_line = str;
	while ((str = get_next_line(request_msg)) != "\n\n")
	{
		key = ft_strtok(str, " :");
		value = str;
		_header_field[key] = value;
	}
}

client::client(int socket, int port)
{
	std::cout << "constructor" << std::endl;
	int ret, bufsize = 2560;
	char buf[bufsize];
	static std::map<int, std::string> map;
	std::string content;
	//std::string str;
	
	ret = read(socket, buf, bufsize - 1);
	socker_num = socket;
	if (ret <= 0)
	{
		//std::cout << "disconnected: " << socket << std::endl;
		FD_CLR(socket, &WEBSERVER->getReadSet());
		FD_CLR(socket, &WEBSERVER->getWriteSet());
		close(socket);
		WEBSERVER->getClientSockets().erase(socket);
		// for (std::map<int, unsigned short>::iterator it = WEBSERVER->getClientSockets().begin();
		// it != WEBSERVER->getClientSockets().end(); it++)
		// {
		// 	std::cout << "아직 client sockets에 존재하는fd들 : " << it->first << std::endl;
		// }
		return;
	}
	buf[ret] = 0;
	map[socket] += std::string(buf);
	int pos;
	if ((pos = ft_contain(map[socket], "\r\n\r\n")) != -1)// 헤더의 끝
	{
		if (_header_field.find("Host") == _header_field.end())
			parse_msg(map[socket]);
		if (_header_field.find("Content-Length") != _header_field.end())
		{
			int length = atoi(_header_field["Content-Lentgth"].c_str());
			if (length > (map[socket].size() - pos))
			{
				flag[socket] = false;
				return ;
			}
			else
			{
				_header_field["body"] =  map[socket].substr(pos, map[socket].size() - pos);
				flag[socket] = true;
			}
		}
		else if (_header_field.find("Transfer-Encoding") != _header_field.end())
		{
			if (map[socket][map[socket].size() - 1] == '0')
			{
				chunk_check(map[socket], pos);
				_header_field["body"] =  map[socket].substr(pos, map[socket].size() - pos);
				flag[socket] = true;
			}
			else
			{
				flag[socket] = false;
				return ;
			}
		}
		else
			flag[socket] = true;
		_info = msg_checker().check(_first_line, _header_field, port);//content를 check()함수에 넘겨주어야함
		map[socket].clear();
	}
	else // 더 읽어들여야함
	{
		flag[socket] = false;
		return ;
	}
}

void client::chunk_check(std::string &src, int pos)
{
	int start = pos;
	int end = src.find_first_of("\r\n", start);
	long len;
	while (1)
	{
		break;
		end = src.find_first_of("\n", start);
		len = ft::hexaStringToLong(src.substr(start, end - start));
		start = end + 1;
		end = src.find_first_of("\r", start) - 2;
		if (len == (end - start + 1))
			std::cout << "ok" << std::endl;
		else
			std::cout << "error" << std::endl;
	}
}
std::string client::get_response()
{
	std::cout << flag[socker_num] << std::endl;
	if (!flag[socker_num])
		return std::string();
	std::ifstream		file;
	std::stringstream	buffer;
	std::string body = "";
	unsigned int s;
	std::string ret = "";
	std::string _abs_path = "";
	if (_info.autoindex && _info.status == "404")
	{
		ret += std::string("HTTP/1.1 ") + "200" + std::string(" ") + "OK" + std::string("\r\n");
		ret += std::string("Server: 42Webserv/1.0\r\n");
		ret += std::string("Date: ") + currentDateTime()+ std::string("\r\n");
		body = _autoindex();
		s = body.size();
		ret += std::string("content-length: ")+ std::to_string(s) + std::string("\r\n");
		ret += std::string("\r\n");
		ret += body;
		return ret;
	}
	ret += std::string("HTTP/1.1 ") + _info.status + std::string(" ") + ft::err().get_err(_info.status) + std::string("\r\n");
	ret += std::string("Server: 42Webserv/1.0\r\n");
	ret += std::string("Date: ") + currentDateTime()+ std::string("\r\n");
	if (_info.status == "404" || _info.status == "403" || _info.status == "405" || _info.status == "414")
	{	
		ret += std::string("Content-type: text/html; charset=UTF-8\r\n");
		_abs_path += "." + _info.error_pages[atoi(_info.status.c_str())];
	}
	else if (_info.status == "301")
	{
		ret += std::string("Location: ") + _info.url_abs_path + std::string("\r\n");
		ret += std::string("\r\n");
		return ret;
	}
	else if (_info.status == "501")
	{
		ret += std::string("\r\n");
		return ret;
	}
	else
	{
		ret += std::string("Content-type: ") + ft::mime().get_mime_type(_info.extention) + std::string("; charset=UTF-8\r\n");// charset=UTF-8 이 부분 없으면 안됨(웹페이지가 불안정하게 표시될 수 있음)
		_abs_path += "." + _info.url_abs_path;
	}
	if (_info.is_cgi)
	{
		body = cgi_process();
		_info.is_cgi = false;
	}
	else
	{
		file.open(_abs_path.c_str(), std::ifstream::in);
		file.is_open();
		buffer << file.rdbuf();
		body = buffer.str();
		file.close();
	}

	s = body.size();
	ret += std::string("content-length: ")+ std::to_string(s) + std::string("\r\n");
	ret += std::string("\r\n");
	ret += body;

	return ret;
}

std::string client::get_location_header()
{
	std::string ret;
	std::string _status = std::string("301");
	ret += std::string("HTTP/1.1 ") + _status + std::string("OK\r\n");
	ret += std::string("Location: ") + _info.url_abs_path;

	return ret;
}

std::string client::cgi_process()
{
    std::string ret;
    char    *argv[3];
    int     pip[2];
    argv[0] = strdup(_info.cgi_path.c_str());
    argv[1] = strdup(("." + _info.url_abs_path).c_str());
	argv[2] = NULL;

    char **env = ft::env("0", _info.extention, _info.url_abs_path, _info.query,\
    		_info.method,_info.host, std::to_string(_info.port), _info.version).get_env();

    int nbytes;
    char inbuf[200];
    if (pipe(pip) != 0)
    {   
        std::cout << "pipe() error" << std::endl; 
        return 0;
    }
    pid_t pid = fork();

    if (pid == 0) 
    {
        dup2(pip[1], 1);
        close(pip[0]);
        close(pip[1]);
        if (-1 == execve(argv[0], argv, env))
			write(2, "execve error\n", 13);
        exit(-1);
    }
    else if (pid > 0) 
    {
        int status;
        
        close(pip[1]);
		waitpid(pid, &status, 0);
        while ((nbytes = read(pip[0], inbuf, 199)) != 0)
        {
            inbuf[nbytes] = 0;
            ret += inbuf;
        }
		close(pip[0]);
    }
    else
    {
        std::cout << "fork error " << std::endl;
        exit (1);
    }
    free(argv[0]);
    free(argv[1]);
    for (int i = 0; i < 17; i++)
        free(env[i]);

    std::string::size_type idx = 0;
    while (idx < ret.size())
    {
        idx = ret.find_first_of("\n", idx);
        if (ret[idx + 1] == '\r')
        {
            ret.erase(0, idx + 3);
            break;
        }
        idx++;
    }
    return ret;
}

msg_checker::msg_checker()
{
	info.status = "200";
}

msg_checker::~msg_checker() {}


int client::getSockNum()
{
	return this->socker_num;
}