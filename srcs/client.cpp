#include "../includes/client.hpp"
#include "../includes/Webserver.hpp"
#include <cstdlib>
#include <cstdio>
# include <sys/types.h>
# include <sys/wait.h>
#include <fcntl.h>

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
	int ret, bufsize = 2560;
	char buf[bufsize];
	std::string str;

	while (1)
	{
		ret = read(socket, buf, 1);
		socker_num = ret;
		if (ret == 0)
		{
			//std::cout << "disconnected: " << socket << std::endl;
			FD_CLR(socket, &WEBSERVER->getReadSet());
			close(socket);
			WEBSERVER->getClientSockets().erase(socket);
			// for (std::map<int, unsigned short>::iterator it = WEBSERVER->getClientSockets().begin();
			// 	it != WEBSERVER->getClientSockets().end(); it++)
			// {
			// 	std::cout << "아직 client sockets에 존재하는것들 : " << it->first << std::endl;
			// }
			return;
		}
		buf[ret] = 0;
		str += std::string(buf);
		if (ft_contain(str, "\r\n\r\n"))// 헤더의 끝
		{
			parse_msg(str);
			std::string content;
			if (_header_field.find("Content-Length") != _header_field.end())
			{
				int length = atoi(_header_field["Content-Lentgth"].c_str());
				ret = 0;
				while (length > 0)
				{
					ret = read(socket, buf, bufsize);
					length -= ret;
					buf[ret] = 0;
					content += std::string(buf);
				}
				for (std::string::iterator i = content.begin(); i != content.end(); i++)
					if (*i == '\n' || *i == '\r')
						length++;
				while (length > 0)
				{
					ret = read(socket, buf, bufsize);
					length -= ret;
					buf[ret] = 0;
					content += std::string(buf);
				}
			}
			else if (_header_field.find("Transfer-Encoding") != _header_field.end())
			{
				std::string temp;
				unsigned int chunk;
				while (1)
				{
					while (1)
					{
						ret = read(socket, buf, 1);
						buf[ret] = 0;
						temp += std::string(buf);
						if (atoi(temp.c_str()) == 0)
						{
							chunk = 0;
							break;
						}
						if (temp.find("\n") != std::string::npos) //find("\r\n")이 아니라 find("\n")이 맞음
						{
							chunk = atoi(temp.c_str());
							break;
						}
					}
					temp.clear();
					if (chunk == 0)
						break;
					while (chunk > 0)
					{
						ret = read(socket, buf, 1);
						if (ret == 1 && (buf[0] == '\n' || buf[0] == '\r'))
							continue;;
						buf[ret] = 0;
						content += std::string(buf);
						chunk -= ret;
					}
					if (1 == recv(socket, buf, 1, MSG_PEEK | MSG_DONTWAIT))
					{
						buf[recv(socket, buf, 1, 0)] = 0;
						content += buf;
					}
				}
			}
			_info = msg_checker().check(_first_line, _header_field, port);//content를 check()함수에 넘겨주어야함
			break;
		}
	}
}

// length 스트링의 넣기 다쓰고 i < 17 까지 free해주기/
//char **env = ft::env("length", _info.extention, _info.url_abs_path, _info.query,
//		_info.method,_info.host, std::to_string(_info.port), _info.version).get_env();

std::string client::get_response()
{
	std::string ret;
	std::string _status = std::string("200");
	ret += std::string("HTTP/1.1 ") + _status + std::string("OK\r\n");
	ret += std::string("Content-type: text/html; charset=UTF-8\r\n");// charset=UTF-8 이 부분 없으면 안됨(웹페이지가 불안정하게 표시될 수 있음)
	std::string _abs_path = "." + _info.url_abs_path;
	int fd = open(_abs_path.c_str(), O_RDONLY);
	char buf[1000];
	int read_size;
	std::string body;
	if (!_info.cgi_path.empty())
	{
		std::cout << "cgi" << std::endl;
		body = cgi_process();
	}
	else
	{
		while ((read_size = read(fd, buf, 999)) != 0)
		{
			buf[read_size] = 0;
			body += std::string(buf);
		}
	}
	close(fd);
	unsigned int s = body.size();
	/*for (std::string::iterator i = body.begin(); i != body.end(); i++)//개행문자의 개수 다 빼줘야할듯
	{
		if (*i == '\n' || *i == '\r')
			s--;
	}*/ 
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
	//std::cout << argv[0] << std::endl << argv[1] << std::endl;
	argv[2] = NULL;

    //argv[0] = "";
    //argv[1] = _info.url_abs_path;
    //argv[0] = strdup("/bin/ls");
    //argv[1] = NULL;
    //argv[2] = NULL;
    char **env = ft::env("2000", _info.extention, _info.url_abs_path, _info.query,\
    		_info.method,_info.host, std::to_string(_info.port), _info.version).get_env();

	
	for (int i =0 ; i < 17; i++)
		std::cout << env[i] << std::endl;
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
		write(2, "dddd\n", 5);
        if (-1 == execve(argv[0], argv, env))
			write(2, "execve error\n", 13);
        exit(-1);
    }
    else if (pid > 0) 
    {
        int status;
        
        close(pip[1]);
		waitpid(pid, &status, 0);
		std::cout << "11111111111111" << std::endl;
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
    /*free(argv[0]);
    free(argv[1]);
    for (int i = 0; i < 18; i++)
        free(env[i]);*/
	/*std::cout << "*********************** " << std::endl;
	std::cout << ret << std::endl;
	std::cout << "*********************** " << std::endl;*/

    std::string::size_type idx = 0;
    while (idx < ret.size())
    {
        idx = ret.find_first_of("\n", idx);
        if (ret[idx + 1] == '\n')
        {
            ret.erase(0, idx + 2);
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

//경로만 오면 인덱스 붙여주고 경로에 파일이 없으면 에러
void msg_checker::check_indexfile(std::string& root, std::vector<std::string> v_index)
{
	std::string tem = root;
	std::string tem_index;

	// 파일명이 있을때
	if (tem.find('.') != std::string::npos)
	{
		tem = "." + tem;
		if (access(tem.c_str(), 0) == 0)
			return ;
		else
		{
			// error 코드
			info.status = "404";
			return ;	
		}
	}
	else // 경로만 있을때
	{
		if (tem.back() != '/')
			tem.push_back('/');
		//index가 있는지 없지 if문 만들고 그안에 넣기
		for (std::size_t i = 0;  i != v_index.size(); i++)
		{
			tem_index = tem + v_index[i];
			tem_index = "." + tem_index;
			if (access(tem_index.c_str(), 0) == 0)
			{
				root.clear();
				root = tem_index.substr(1);		
				return ;			
			}
			tem_index.clear();
		}
		info.status = "404";
		return;
	}
}

int client::getSockNum()
{
	return this->socker_num;
}