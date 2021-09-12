#include "../includes/client.hpp"
#include "../includes/Webserver.hpp"

std::string client::_autoindex()
{
	DIR *dir;
    struct dirent *ent;
	std::string t = _info.location_uri;
	std::string t2;
	t2.assign(t, t.find_last_of("/"), t.size() - t.find_last_of("/"));
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
		{
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
					test.erase(test.begin() + test.size() - 1);
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
		closedir (dir);
    }
	else
        perror ("");
	return ret;
}

void client::parse_msg(std::string &src)
{
	std::string str;
	std::string temp;
	std::string key;
	std::string value;
	std::string request_msg = src;
	str = get_next_line(request_msg);
	_first_line = str;

	while ((str = get_next_line(request_msg)) != "\n\n")
	{
		key = ft_strtok(str, " :");
		value = str;
		_header_field[key] = value;
	}

}

std::string chunk_check(std::string &src, int pos)
{
	std::string::size_type start = (std::string::size_type)pos;
	std::string::size_type end;
	std::string::size_type len;
	std::string body;

	while (1)
	{
		end = ft::find_first_of(src.c_str(), "\r", start) + 1;
		len = hexaStringToLong(src.substr(start, end - start -1));
		if (end == std::string::npos || (src[start] == '0' && start == src.size() - 1))
			break;
		start = end + 1;
		end = ft::find_first_of(src.c_str(), "\r", start) - 1;
		if (len == (end - start + 1))
			std::cout << "ok\n" << std::endl;
		else
			std::cout << "error\n" << std::endl;
		body += src.substr(start, end - start + 1);
		start = end + 3;
	}
	return body;
}


client::client(int socket, int port)
{
	int ret, bufsize = 4096;
	char buf[bufsize];
	static std::map<int, std::string> map;
	std::string content;
	int pos;
	pos = ft_contain(map[socket], "\r\n\r\n");
	ret = recv(socket, buf, bufsize - 1, 0);
	socket_num = socket;
	is_read_end = false;
	if (ret <= 0)
	{
		std::cout << "disconnected: " << socket << std::endl;
		FD_CLR(socket, &WEBSERVER->getReadSet());
		FD_CLR(socket, &WEBSERVER->getWriteSet());
		close(socket);
		WEBSERVER->getClientSockets().erase(socket);
		for (std::map<int, unsigned short>::iterator it = WEBSERVER->getClientSockets().begin();
		it != WEBSERVER->getClientSockets().end(); it++)
		{
			std::cout << "아직 client sockets에 존재하는fd들 : " << it->first << std::endl;
		}
		is_read_end = true;
		return;
	}
	for(int i = 0; i < ret; i++)
		map[socket] += buf[i];
	buf[ret] = 0;
	int length;
	if ((pos = ft_contain(map[socket], "\r\n\r\n")) != -1)// 헤더의 끝
	{
		if (_header_field.find("Host") == _header_field.end())
		{
			parse_msg(map[socket]);
			length = atoi(_header_field["Content-Length"].c_str());
		}
		if (_header_field.find("Content-Length") != _header_field.end())
		{
			unsigned long max_size = WEBSERVER->getServerList()[0].getClientMaxBodySize();
			if (max_size < map[socket].size() - pos)
			{
				_header_field["body"] =  map[socket].substr(pos, map[socket].size() - pos);
				flag[socket] = true;
			}
			else if ((unsigned long)length > (map[socket].size() - pos))
			{
				flag[socket] = false;
				return ;
			}
			else
			{
				_header_field["body"] =  map[socket].substr(pos, map[socket].size() - pos);
				// if (length > bufsize)
				// {
				// 	FILE *fp_bin = fopen("./var/www/html/upload/ex2.jpg", "wb");
				// 	std::cout << "body size: " << _header_field["body"].size() << std::endl;
				// 	int i = 0;
				// 	for (std::string::iterator it = _header_field["body"].begin();
				// 			it != _header_field["body"].end(); it++)
				// 	{
				// 		buf2[0] = *it;
				// 		fwrite(buf2, 1, 1, fp_bin);
				// 		i++;
				// 	}
				// 	fclose(fp_bin);
				// 	std::cout << "i : " << i << std::endl;
				// }
				map[socket].clear();
				flag[socket] = true;
			}
		}
		else if (_header_field.find("Transfer-Encoding") != _header_field.end())
		{
			if (map[socket][map[socket].size() - 1] == '0')
			{
				_header_field["body"] =  chunk_check(map[socket], pos);
				flag[socket] = true;
				map[socket].clear();
			}
			else
			{
				flag[socket] = false;
				return ;
			}
		}
		else
		{
			flag[socket] = true;
			map[socket].clear();
		}
		if (flag[socket] == true)
		{
			_info = msg_checker().check(_first_line, _header_field, port);//content를 check()함수에 넘겨주어야함
			std::cout << "finish check" << std::endl;
			map[socket].clear();
		}
	}
	else // 더 읽어들여야함
	{
		flag[socket] = false;
		return ;
	}
}

void	client::post_upload()
{
	if (_info.body == "\r\n")
	{
		_info.post_err = true;
		_info.query = "erro=파일을 선택해 주세요";
		return ;
	}
	if (_info.body_size > (int)_info.max_body_size)
	{
		_info.post_err = true;
		_info.query = "erro=파일 크기 제한 0.5M!!";
		return ;
	}
	if (_info.extention != ".jpg" && _info.extention != ".png" && _info.extention != ".JPEG"
		&& _info.extention != ".php" && _info.extention != ".html" && _info.extention != ".htm")
	{
		_info.post_err = true;
		_info.query = "erro=업로드 가능 파일 형식 => jpg/jpeg/png, html/htm, php";
		return ;
	}
	if (_info.post_err == false)
	{
		std::string _abs_path = "." + _info.url_abs_path;
		std::ofstream file(_abs_path.c_str());
		if (file.is_open())
			file << _info.body;
		else
		{
			_info.post_err = true;
			_info.query = "erro=파일 저장에 실패 하였습니다";
		}
		file.close();
		return ;
	}
}

std::string client::get_response()
{
	if (!flag[socket_num])
		return std::string();
	std::cout << "get_response" << std::endl;
	std::cout << _info.body_size << std::endl;
	std::ifstream		file;
	std::stringstream	buffer;
	std::string body = "";
	unsigned int s;
	std::string ret = "";
	std::string _abs_path = "";
	if (_info.autoindex && _info.status == "404" && _info.same_location)
	{
		ret += std::string("HTTP/1.1 ") + "200" + std::string(" ") + "OK" + std::string("\r\n");
		ret += std::string("Server: 42Webserv/1.0\r\n");
		ret += std::string("Date: ") + currentDateTime()+ std::string("\r\n");
		body = _autoindex();
		s = body.size();
		std::stringstream ss;
		ss << s;
		ret += std::string("content-length: ")+ ss.str() + std::string("\r\n");
		ret += std::string("\r\n");
		ret += body;
		return ret;
	}
	if (_info.method == "DELETE" && _info.status == "204")
		delet_file();
	if (_info.method == "POST" && _info.is_file && _info.status != "204")
		client::post_upload();
	ret += std::string("HTTP/1.1 ") + _info.status + std::string(" ") + ft::err().get_err(_info.status) + std::string("\r\n");
	ret += std::string("Server: 42Webserv/1.0\r\n");
	ret += std::string("Date: ") + currentDateTime()+ std::string("\r\n");
	if (_info.status == "404" || _info.status == "403" || _info.status == "405" || _info.status == "414")
	{
		ret += std::string("Content-type: text/html; charset=UTF-8\r\n");
		_abs_path += "." + _info.error_pages[atoi(_info.status.c_str())];
	}
	else if (_info.method == "DELETE" && _info.status == "204")
	{
		ret += std::string("\r\n");
			return ret;
	}
	else if (_info.status == "301")
	{
		std::cout << _info.url_abs_path  << std::endl;
		ret += std::string("Location: ") + _info.url_abs_path + std::string("\r\n");
	}
	else if (_info.method == "POST" && _info.is_file)
	{
		if(!_info.post_err)
		{
			if (!_info.is_cgi)
				_abs_path = "." + _info.url_abs_path;
			ret += std::string("Location: ./upload/") + _info.body_filename + std::string("\r\n");
		}
		else
		{
			_info.url_abs_path.clear();
			_info.url_abs_path = "/var/www/html/upload/default.php";
			_info.extention = ".php";
			_info.is_cgi = true;
			ret += std::string("Location: ./var/www/html/upload/default.php") + std::string("\r\n");
		}
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
	std::stringstream ss;
	ss << s;
	ret += std::string("content-length: ")+ ss.str() + std::string("\r\n");
	ret += std::string("\r\n");
	ret += body;
	return ret;
}

void client::delet_file()
{
	std::string path = "." + _info.url_abs_path;
	if (remove(path.c_str()) == 0)
		_info.status = "204";
	else
		_info.status = "403";
}

std::string client::cgi_process()
{
    std::string ret;
    char    *argv[3];
    int     pip[2];
    argv[0] = strdup(_info.cgi_path.c_str());
    argv[1] = strdup(("." + _info.url_abs_path).c_str());
	argv[2] = NULL;
	std::stringstream ss;
	ss << _info.port;

    char **env = ft::env("0", _info.extention, _info.url_abs_path, _info.query,\
    		_info.method,_info.host, ss.str(), _info.version).get_env();

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
        idx = ft::find_first_of(ret.c_str(), "\n", idx);
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
	return this->socket_num;
}

bool client::isReadEnd()
{
	return this->is_read_end;
}
