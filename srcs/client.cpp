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
    {
		std::cout << "dir open error" << std::endl;
	    exit(-1);
	}
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

std::string client::chunk_check(std::string &src, int pos)
{
	std::string::size_type start = (std::string::size_type)pos;
	std::string::size_type end;
	std::string::size_type len;
	std::string body;
	while (1)
	{
		end = ft::find_first_of(src.c_str(), "\r", start) + 1;
		len = hexaStringToLong(src.substr(start, end - start -1));
		if (end == std::string::npos || ( (start == end - 2) && src[end - 2] == '0' && src[end - 1] == '\r' && src[end] == '\n'))
			break;
		start = end + 1;
		end = start + len - 1;
		if (len != (end - start + 1))
		{
			chunk_error = true;
			return std::string();
		}
		body += src.substr(start, end - start + 1);
		start = end + 3;
	}
	return body;
}

client::client(int socket, int port):chunk_error(false)
{
	int ret, bufsize = 4096;
	char buf[bufsize];
	static std::map<int, std::string> map;
	std::string content;
	int pos;
	int length = 0;
	pos = ft_contain(map[socket], "\r\n\r\n");
	ret = read(socket, buf, bufsize - 1);
	socket_num = socket;
	is_read_end = false;
	for(int i = 0; i < ret; i++)
		map[socket] += buf[i];
	if (ret <= 0)
	{
		//std::cout << "disconnected: " << socket << std::endl;
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
	buf[ret] = 0;
	if ((pos = ft_contain(map[socket], "\r\n\r\n")) != -1)// 헤더의 끝
	{
		if (_header_field.find("Host") == _header_field.end())
		{
			parse_msg(map[socket]);
			length = atoi(_header_field["Content-Length"].c_str());
		}
		
		if (_header_field.find("Transfer-Encoding") != _header_field.end())
		{
			if (map[socket][map[socket].size() - 3] == '0' && map[socket][map[socket].size() - 2] == '\r' \
				&& map[socket][map[socket].size() - 1] == '\n')
			{
				_header_field["body"] =  chunk_check(map[socket], pos);
				_flag[socket] = true;
				map[socket].clear();
			}
			else
			{
				_flag[socket] = false;
				return ;
			}
		}
		else if (_header_field.find("Content-Length") != _header_field.end())
		{
			//std::cout <<"length: " << length << std::endl;
			//std::cout <<"map[socket].size(): " << map[socket].size() << std::endl;
			//std::cout <<"pos: " << pos << std::endl;
			if (length < (int)(map[socket].size() - pos))
			{
				bodySizeError(map, pos, socket, port, "400");
				return;
			}
			for (std::map<int, Server>::iterator it = WEBSERVER->getServerList().begin();
				it != WEBSERVER->getServerList().end(); it++)
			{
				for(std::vector<unsigned short>::iterator it2 = it->second.getPorts().begin();
					it2 != it->second.getPorts().end(); it2++)
				{
					if (*it2 == (unsigned short)port)
					{
						if ((map[socket].size() - pos) > it->second.getClientMaxBodySize())
						{
							bodySizeError(map, pos, socket, port, "413");
							return;
						}
					}
				}
			}
			if ((unsigned long)length > (map[socket].size() - pos))
			{
				//std::cout << "length에 도달못하여 더 읽습니다" << std::endl;
				_flag[socket] = false;
				return;
			}
			else
			{
				_header_field["body"] =  map[socket].substr(pos, map[socket].size() - pos);
				map[socket].clear();
				_flag[socket] = true;
			}
		}
		else
		{
			_flag[socket] = true;
			map[socket].clear();
		}
		if (_flag[socket] == true)
		{
			_info = msg_checker().check(_first_line, _header_field, port);
			map[socket].clear();
		}
	}
	else
	{
		_flag[socket] = false;
		return ;
	}
}


std::pair<int, std::string> client::get_response()
{
	
	std::ifstream		file;
	std::stringstream	buffer;
	std::string body = "";
	unsigned int s;
	std::string ret = "";
	cgi_flag = false;
	
	return_value.first = 0;
	client::exe_method();
	int post_flag = 0;
	if (return_value.first != 0)
	{
		post_flag = 1;
	}
	ret += std::string("Server: 42Webserv/1.0\r\n");
	ret += std::string("Date: ") + currentDateTime()+ std::string("\r\n");

	if (_info.status == "400" || _info.status == "404" || _info.status == "403" || _info.status == "405" || _info.status == "414" || _info.status == "413")
	{
		ret += std::string("Content-type: text/html; charset=UTF-8\r\n");
		_abs_path += "." + _info.error_pages[atoi(_info.status.c_str())];
		_info.is_cgi = false;
	}
	else if (_info.method == "DELETE" && _info.status == "204")
	{
		_abs_path = "./var/www/html/delete.html";
	}
	else if (_info.status == "301")
	{
		ret += std::string("Location: .") + _info.url_abs_path + std::string("\r\n");
		_abs_path = "./var/www/html/errors/default.html";
	}
	else if (_info.method == "POST" && _info.status == "201")
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
			ret += std::string("Location: ./upload/default.php") + std::string("\r\n");
		}
	}
	else if (_info.status == "501")
	{
		_abs_path = "./var/www/html/errors/default.html";
		_info.is_cgi = false;
	}
	else if (!_info.check_autoindex)
	{
		ret += std::string("Content-type: ") + ft::mime().get_mime_type(_info.extention) + std::string("; charset=UTF-8\r\n");// charset=UTF-8 이 부분 없으면 안됨(웹페이지가 불안정하게 표시될 수 있음)
		_abs_path += "." + _info.url_abs_path;
	}

	//////////파일 읽기/////////////
	if (_info.check_autoindex)
	{
		return_value.first = socket_num;
		body = _autoindex();
	}
	else if (_info.is_cgi)
	{
		cgi_flag = true;
		int r = cgi_process();
		if (r == -1)
			exit(1);
		else
			return_value.first = r;
	}
	else
	{
		if (post_flag == 1)
		{
			//std::cout << "post flag" << std::endl;
			ret += "\r\n";
			if (!_info.post_err)
				ret += _info.body;
			else
				ret += _info.query;
		}
		else
		{
			int te = open(_abs_path.c_str(), O_RDWR | O_CREAT | S_IRWXU, 0774);
			fcntl(te, F_SETFL, O_NONBLOCK);
			return_value.first = te;
		}
	}

	ret.insert(0, std::string("HTTP/1.1 ") + _info.status + std::string(" ") + ft::err().get_err(_info.status) + std::string("\r\n"));
	
	if(return_value.first == socket_num)
	{
		s = body.size();
		std::stringstream ss;
		ss << s;
		ret += std::string("Content-Length: ")+ ss.str() + std::string("\r\n");
		ret += std::string("\r\n");
		ret += body;
	}
	return_value.second = ret;
	return return_value;
}

std::string client::getMethod()
{
	return _info.method;
}
void client::exe_method()
{

	if (_info.autoindex && _info.status == "404" && _info.same_location)
	{
		_info.status = "200";
		_info.check_autoindex = true;
	}
	if (_info.method == "DELETE" && _info.status == "204")
		delet_file();
	if (_info.method == "POST" && _info.status == "201" && _info.status != "204" )
		post_upload();
	else
		_info.post_err = true;
		
	if (chunk_error)
	{
		_info.status = "400";
		chunk_error = false;
	}
}

void	client::post_upload()
{
	if (_info.body == "\r\n")
	{
		_info.post_err = true;
		_info.query = "erro=파일을 선택해 주세요";
		_info.body.clear();
		return ;
	}
	if (_info.body_size > (int)500000)
	{
		_info.post_err = true;
		_info.query = "erro=파일 크기 제한 0.5M!!";
		return ;
	}
	if (_info.extention != ".jpg" && _info.extention != ".png" && _info.extention != ".JPEG"
		&& _info.extention != ".html" && _info.extention != ".htm")
	{
		_info.post_err = true;
		_info.query = "erro=업로드 가능 파일 형식 => jpg/jpeg/png, html/htm";
		return ;
	}
	if (_info.post_err == false)
	{
		_abs_path = "." + _info.url_abs_path;
		std::cout << _abs_path << std::endl;
		int f = open(_abs_path.c_str(), O_RDWR | O_CREAT | O_TRUNC | S_IRWXU, 0774);
		fcntl(f, F_SETFL, O_NONBLOCK);
		//std::cout << "396줄에서 체크해봅니다" << f << std::endl;
		return_value.first = f;
		return ;
	}
}

void client::delet_file()
{
	std::string path = "." + _info.url_abs_path;
	if (remove(path.c_str()) == 0)
		_info.status = "204";
	else
		_info.status = "403";
}

int client::cgi_process()
{
    std::string ret;
    char    *argv[3];
    int     pip[2];
    argv[0] = strdup(_info.cgi_path.c_str());
    argv[1] = strdup(("." + _info.url_abs_path).c_str());
	//std::cout << "file path" << argv[1] << std::endl;
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
		if (status == -1)
		{
			free(argv[0]);
			free(argv[1]);
			for (int i = 0; i < 17; i++)
				free(env[i]);
			return -1;
		}
		else
		{
			free(argv[0]);
			free(argv[1]);
			for (int i = 0; i < 17; i++)
				free(env[i]);
			fcntl(pip[0], F_SETFL, O_NONBLOCK);
			return pip[0];
		}
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
    return -1;
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

void client::bodySizeError(std::map<int, std::string> &map, int pos, int socket, int port, std::string errnum)
{
	_header_field["body"] =  map[socket].substr(pos, map[socket].size() - pos);
	//std::cout << "body size : " << _header_field["body"].size() << std::endl;
	_flag[socket] = true;
	FD_CLR(socket, &WEBSERVER->getReadSet());
	shutdown(socket, SHUT_RD);
	WEBSERVER->getIsSocketEnd()[socket] = true;
	_info = msg_checker().check(_first_line, _header_field, port);
	_info.status = errnum;
	map[socket].clear();
	return;
}