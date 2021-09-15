#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <set>
#include "../includes/Webserver.hpp"
#include "../includes/utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/client.hpp"



Webserver *Webserver::instance;

Webserver::Webserver()
{

}

Webserver::~Webserver()
{
	for (std::map<int, unsigned short>::iterator it = instance->client_sockets.begin();
		it != instance->client_sockets.end(); it++)
	{
		FD_CLR(it->first, &instance->read_set);
		FD_CLR(it->first, &instance->write_set);
		std::cout << "deleted clt socket : " << it->first << std::endl;
		close(it->first);
	}
	for (std::map<int, ServerFD>::iterator it = instance->server_sockets.begin();
		it != instance->server_sockets.end(); it++)
	{
		FD_CLR(it->first, &instance->read_set);
		std::cout << "deleted serv socket : " << it->first << std::endl;
		close(it->first);
	}
}

Webserver *Webserver::getInstance()
{
	if (instance == NULL)
		instance = new Webserver;
	return (instance);
}

std::map<int, Server> &Webserver::getServerList()
{
	return this->server_list;
}

std::set<unsigned short> &Webserver::getListenPort()
{
	return this->listen_port;
}

std::map<int, unsigned short> &Webserver::getClientSockets()
{
	return this->client_sockets;
}

std::map<int, ServerFD> &Webserver::getServerSockets()
{
	return this->server_sockets;
}

std::map<int, bool> &Webserver::getIsSocketEnd()
{
	return this->is_socket_end;
}

fd_set &Webserver::getReadSet()
{
	return this->read_set;
}

fd_set &Webserver::getWriteSet()
{
	return this->write_set;
}

bool Webserver::parsingConfig(const char *config_file)
{
	std::ifstream ifs;
	std::vector<std::string> chunked_config;
	std::string line;
	std::string location_path;
	std::string tmpstr;
	int left_curly_brace;
	int right_curly_brace;
	int index;
	int port;
	bool is_http;
	
	index = -1;
	left_curly_brace = 0;
	right_curly_brace = 0;
	is_http = false;
	ifs.open(config_file);

	if (ifs.fail())
	{
		std::cerr << "Error : config_file open fail" << std::endl;
		return false;
	}
	try
	{
		while (1)
		{
			std::getline(ifs, line);
			ft::split(line, " \t;=", chunked_config);
			if (line.find('{') != std::string::npos)
				left_curly_brace++;
			if (line.find('}') != std::string::npos)
				right_curly_brace++;
			line.clear();
			if (ifs.eof())
				break ;
		}
		if (left_curly_brace != right_curly_brace)
			throw "ERROR: curly_brace does not match";

		for (std::vector<std::string>::iterator iter = chunked_config.begin(); iter != chunked_config.end(); iter++)
		{
			if (*iter == "http")
			{
				if (is_http)
					throw "ERROR: config_file should have only one http block";
				is_http = true;

			}
			if (*iter == "server")
			{
				index++;
				location_path.clear();
			}
			if (*iter == "listen")
			{
				iter++;
				if ((*iter).find(":") == std::string::npos)
					throw "ERROR : host and port must be configured";
				port = atoi((*iter).substr((*iter).find(":") + 1).c_str());
				tmpstr = (*iter).substr(0, (*iter).find(":"));
				if (tmpstr != "localhost" && tmpstr != "127.0.0.1")
					throw "ERROR : host is wrong";
				instance->server_list[index].getPorts().push_back((unsigned short)port);
				instance->server_list[index].setHost("localhost");
				instance->listen_port.insert((unsigned short)port);
				iter++;
				while (*iter == "listen")
				{
					iter++;
					if ((*iter).find(":") == std::string::npos)
						throw "ERROR : host and port must be configured";
					port = atoi((*iter).substr((*iter).find(":") + 1).c_str());
					tmpstr = (*iter).substr(0, (*iter).find(":"));
					for (std::vector<unsigned short>::iterator it = instance->server_list[index].getPorts().begin();
						it != instance->server_list[index].getPorts().end(); it++)
					{
						if ((unsigned short)port == *it)
							throw "ERROR : one server listen on the same port";
					}
					if (tmpstr != "localhost" && tmpstr != "127.0.0.1")
						throw "ERROR : host is wrong";
					instance->server_list[index].getPorts().push_back((unsigned short)port);
					instance->listen_port.insert((unsigned short)port);
					iter++;
				}
				
			}
			if (*iter == "server_name")
			{
				iter++;
				instance->server_list[index].setServerName(*iter);
			}
			else if (*iter == "client_max_body_size")
			{
				iter++;
				int tmp = atoi((*iter).c_str());
				if (tmp <= 0)
					throw "ERROR : client_max_body_size is wrong";
				instance->server_list[index].setClientMaxBodySize(static_cast<unsigned int>(tmp));
				
			}
			else if (*iter == "root")
			{
				iter++;
				if (location_path == "")
					instance->server_list[index].setRoot(*iter);
				else
					instance->server_list[index].getLocations()[location_path].setRoot(*iter);
			}
			else if (*iter == "error_page")
			{
				iter++;
				int key = atoi((*iter).c_str());
				iter++;
				instance->server_list[index].getErrorPages()[key] = *iter; 
			}
			else if (*iter == "location")
			{
				iter++;
				if ((*iter)[0] != '/')
					throw "ERROR : location path is wrong";
				location_path = *iter;
				instance->server_list[index].getLocations()[location_path].setLocationPath(location_path);
				instance->server_list[index].getLocations()[location_path].setAutoIndex(false);
			}
			else if (*iter == "autoindex")
			{
				iter++;
				if (*iter == "off")
					instance->server_list[index].getLocations()[location_path].setAutoIndex(false);
				else if (*iter == "on")
					instance->server_list[index].getLocations()[location_path].setAutoIndex(true);
				else
					throw "ERROR : autoindex is wrong";
			}
			else if (*iter == "limit_except")
			{
				iter++;
				while (ft::isMethods(*iter) && iter != chunked_config.end())
				{
					instance->server_list[index].getLocations()[location_path].getLimitExcept().push_back(*iter);
					iter++;
				}
				if (iter == chunked_config.end())
					break;
				iter--;
			}
			else if (*iter == "cgi_except")
			{
				iter++;
				while ((*iter)[0] == '.')
				{
					instance->server_list[index].getLocations()[location_path].getCigExcept().push_back(*iter);
					iter++;
				}
				iter--;
			}
			else if (*iter == "cgi_path")
			{
				iter++;
				instance->server_list[index].getLocations()[location_path].setCgiPath(*iter);
			}
			else if (*iter == "return")
			{
				iter++;
				iter++;
				instance->server_list[index].getLocations()[location_path].setRedirection(*iter);
			}
			else if (*iter == "index")
			{
				iter++;
				while ((*iter).find('.') != std::string::npos && iter != chunked_config.end())
				{
					if (location_path == "")
						instance->server_list[index].getDifaultFiles().push_back(*iter);
					else
						instance->server_list[index].getLocations()[location_path].getDifaultFiles().push_back(*iter);
					iter++;
				}
				if (iter == chunked_config.end())
					break;
				iter--;
			}
		}
	}
	catch(const char *e)
	{
		std::cerr << e << std::endl;
		ifs.close();
		return false;
	}
	ifs.close();
	return true;
}

void Webserver::error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

bool is_files(int fd, std::vector<int> files)
{
	for (std::vector<int>::iterator iter = files.begin(); iter != files.end(); iter++)
	{
		if (fd == *iter)
			return true;
	}
	return false;
}
void Webserver::initWebServer()
{
 	int clnt_sock;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_size;
	int socketnum = 0;
	bool connected;

	FD_ZERO(&instance->read_set);
	FD_ZERO(&instance->write_set);
	int fd_max;
	int ret;
	int port;
	int select_count = 1;
	//struct timeval timeout;

	
	// serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	// memset(&serv_adr, 0, sizeof(serv_adr));
	// serv_adr.sin_family=AF_INET;
	// serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	// serv_adr.sin_port = htons(20000);
	// if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	// 	error_handling("bind() error");
	// if(listen(serv_sock, 1024)==-1)
	// 	error_handling("listen() error");

	for (std::set<unsigned short>::iterator it = instance->listen_port.begin();
		it != instance->listen_port.end(); it++)
	{
		socketnum = socket(PF_INET, SOCK_STREAM, 0);
		if (socketnum == -1)
			error_handling("socket() error");
		int opt = 1;
		setsockopt(socketnum, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
		setsockopt(socketnum, SOL_SOCKET,  SO_RCVBUF, &opt, sizeof(opt)); 
		instance->server_sockets[socketnum];
		instance->server_sockets[socketnum].socket = socketnum;
		memset(&instance->server_sockets[socketnum].serv_adr, 0, sizeof(instance->server_sockets[socketnum].serv_adr));
		instance->server_sockets[socketnum].serv_adr.sin_family= AF_INET;
		instance->server_sockets[socketnum].serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
		instance->server_sockets[socketnum].serv_adr.sin_port = htons(*it);
		if (bind(instance->server_sockets[socketnum].socket, (struct sockaddr*)&instance->server_sockets[socketnum].serv_adr,
			sizeof(instance->server_sockets[socketnum].serv_adr)) == -1)
		{
			error_handling("bind() error");
		}
		if(listen(socketnum, 1024)==-1)
			error_handling("listen() error");
		fcntl(instance->server_sockets[socketnum].socket, F_SETFL, O_NONBLOCK);
		FD_SET(instance->server_sockets[socketnum].socket, &instance->read_set);
	}

	fd_max = instance->server_sockets.rbegin()->first;
	std::cout << "fd_max : " << fd_max << std::endl;

	//



	std::vector<std::pair<int, int> > sock_file_pair;
	std::map<int, std::string> sock_msg;
	std::map<int, std::string> sock_body;
	std::map<int, msg_checker::return_type> sock_struct;
	std::vector<int> files;


	//
	while(1)
	{
		//timeout.tv_sec = 5;
		//timeout.tv_usec = 500;
		instance->read_temp = instance->read_set;
		instance->write_temp = instance->write_set;
		std::cout << "select count: " << select_count++ <<  std::endl;
		ret = select(fd_max + 1, &instance->read_temp, &instance->write_temp, NULL, 0);

		if (ret < 0)
		{
			printf("select error detached");
			break;
		}
		else if (ret == 0)
		{
			printf("time out!\n");
			continue;
		}
		else
		{
			for (int i = 3; i < fd_max + 1; i++)
			{
				if (FD_ISSET(i, &instance->read_temp))
				{
					connected = false;
					for (std::map<int, ServerFD>::iterator it = instance->server_sockets.begin();
						it != instance->server_sockets.end(); it++)
					{
						if (it->first == i)
						{
							std::cout << "new clnt is connected" << std::endl;
							clnt_adr_size = sizeof(clnt_adr);
							clnt_sock = accept(instance->server_sockets[it->first].socket, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
							if (clnt_sock == -1)
								error_handling("accept() error");
							std::cout << "clnt sock  " << clnt_sock << std::endl;
							instance->client_sockets.insert(std::pair<int, unsigned short>(clnt_sock, it->second.serv_adr.sin_port));
							instance->is_socket_end[clnt_sock] = false;
							std::cout << "accept_clnt_sock: " << clnt_sock << std::endl;
							fcntl(clnt_sock, F_SETFL, O_NONBLOCK);
							FD_SET(clnt_sock, &instance->read_set);
							if (fd_max < instance->client_sockets.rbegin()->first)
								fd_max = clnt_sock;
							connected = true;
							break;
						}
					}
					if (connected != true) //read message
					{
						std::cout << "client socket read : " << i << std::endl;
						port = ntohs(instance->client_sockets[i]);
						//std::cout << "is_files: " << is_files(i, files) << std::endl;
						if (is_files(i, files) == false)
						{
							//std::cout << "socket을 읽고 있습니다" << std::endl;
							client obj(i, port);
							if (obj._flag[i] == false)
								continue;
							if (obj.isReadEnd() == true)
								continue;
							std::cout << "socket을 다 읽었습니다" << std::endl;
							std::pair<int, std::string> t = obj.get_response();
							//std::cout << "get response" << std::endl;
							sock_file_pair.push_back(std::pair<int, int>(i, t.first));
							files.push_back(t.first);
							sock_msg[i] = t.second;//(가)
							if (t.first == i)
							{
								//std::cout << "t_first == i" << std::endl;
								response_list[i] = t.second;//(나)
								FD_SET(i, &instance->write_set);//socket_write 갈 예정
								if (fd_max < i)
									fd_max = i + 1;
							}
							else
							{
								if (obj.getMethod() == "POST")
								{
									//std::cout << "POST" << std::endl;
									if (obj.cgi_flag)
									{
										FD_SET(t.first, &instance->read_set);//나중에 close필요
										if (fd_max < t.first)
											fd_max = t.first + 1;
									}
									else if (obj._info.post_err == false)
									{
										/*if (obj._info.post_err == true)
										{
											FD_SET(i, &instance->write_set);//에러 메시지를 보낼것
											response_list[i] = t.second;//(나)
										}*/
										//std::cout << "cgi는 아  님    " << t.first <<  std::endl;
										FD_SET(t.first, &instance->write_set);//나중에 close필요
										int position = sock_msg[i].find_first_of("\n");
										position++;
										int pp = ft::ft_contain(sock_msg[i], "\r\n\r\n");
										int s = sock_msg[i].size() - pp;
										//std::cout << s << std::endl;
										std::stringstream ss;
										ss << s;
										sock_msg[i].insert(position, std::string("content-length: ")+ ss.str() + std::string("\r\n"));
										response_list[i] = sock_msg[i];
										//int zz = open("test.txt", O_RDWR | O_CREAT);
										//write(zz, sock_msg[i].c_str(), sock_msg[i].size());
										//close(zz);
										if (fd_max < t.first)
											fd_max = t.first + 1;
										
										FD_SET(i, &instance->write_set);//이건 close해줄 필요가 없음
										if (fd_max < i)
											fd_max = i + 1;
										//std::cout << "써야할 파일을 등록 완료했습니다" << std::endl;
									}
									else
									{
										FD_SET(t.first, &instance->read_set);
										if (fd_max < t.first)
											fd_max = t.first + 1;
									}
								}
								else
								{
									//std::cout << "이 파일을 읽어야합니다" << t.first << std::endl;
									if (obj._info.post_err)
										sock_msg[i] += "\r\n";
									FD_SET(t.first, &instance->read_set);
									if (fd_max < t.first)
										fd_max = t.first + 1;
								}
							}

							/*if (t.empty())
								continue;*/
							//response_list[i] = t;
							//FD_SET(i, &instance->write_set);
						}
						else//file fd(read)
						{
							int file = i;
							int socket = -1;
							for (std::vector<std::pair<int, int> >::iterator iter = sock_file_pair.begin(); iter != sock_file_pair.end(); iter++)
							{
								if (iter->second == i)
									socket = iter->first; 
							}
							//if (socket == -1)
								//std::cout << "찾지 못함" << std::endl;
							char b[2560];
							int r_size = read(i, b, 2560);
							//std::cout << file <<"번 file을 읽고 있습니다" << std::endl;
							for (int idx = 0; idx < r_size; idx++)
								sock_body[socket].push_back(b[idx]);
							
							if (r_size != 2560)
							{
								//std::cout << file <<"번 file을 읽기가 끝났습니다" << std::endl;
								//std::cout << "size: " << sock_body[socket].size() << std::endl;
								//std::cout << sock_body[socket] << std::endl;
								FD_CLR(file, &instance->read_set);
								close(file);
								int ppp = ft::ft_contain(sock_body[socket], "\r\n\r\n");
								//int powerd = ft::ft_contain(sock_body[socket], "X-Powerded-By");
								if (ppp != -1)
									sock_body[socket] = sock_body[socket].substr(ppp, sock_body[socket].size() - ppp);
								
								int s = sock_body[socket].size();
								std::stringstream ss;
								ss << s;
								//std::cout << "ss : "<< ss <<std::endl;
								sock_msg[socket] += std::string("content-length: ")+ ss.str() + std::string("\r\n");
								sock_msg[socket] += std::string("\r\n");
								int as = open("test.txt", O_RDWR | O_CREAT | O_TRUNC);
								write(as, sock_msg[socket].c_str(), sock_msg[socket].size());
								write(as, "|||||\n", 6);
								write(as, sock_body[socket].c_str(), sock_body[socket].size());
								//std::cout << "size: " << sock_body[socket].size() << std::endl;
								close(as);
								response_list[socket] = sock_msg[socket] + sock_body[socket];//(나)
								sock_body[socket].clear();
								sock_msg[socket].clear();
								FD_SET(socket, &instance->write_set);//socket_write 갈 예정
								if (fd_max < socket)
									fd_max = socket + 1;
								//std::cout << "write set에 등록 완료" << std::endl;
							}
							//else
								//std::cout << "nothing can do" << std::endl;
							/*
							FILE *fp = fopen("hello.txt", "r");    // hello.txt 파일을 읽기 모드(r)로 열기.
																// 파일 포인터를 반환
							int con_len;
							fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시킴
							con_len = ftell(fp);          // 파일 포인터의 현재 위치를 얻음
							//printf("%d\n", size);      // 13
							fclose(fp);
							*/
			
						}
					}
				}
				if (FD_ISSET(i, &instance->write_temp) && !FD_ISSET(i, &instance->read_temp)) //write 
				{
					if (is_files(i, files) == false)
					{
						//std::cout << "client socket write : " << i << std::endl;
						
						if (write(i, response_list[i].c_str(), response_list[i].size()) <= 0)
						{
							FD_CLR(i, &instance->write_set);
							FD_CLR(i, &instance->read_set);
							close(i);
							instance->client_sockets.erase(i);
						}
						else
						{
							std::cout << "모두 보냄" <<std::endl;
							std::cout << response_list[i] << std::endl;
							if (instance->is_socket_end[i] == true)
							{
								FD_CLR(i, &instance->write_set);
								//close(i);
								instance->client_sockets.erase(i);
							}
							else
								FD_CLR(i, &instance->write_set);
						}
						response_list[i].clear();
					}
					else//files
					{
						//std::cout << "file fd write : " << i << std::endl;
						int file = i;
						int socket = -1;
						for (std::vector<std::pair<int, int> >::iterator iter = sock_file_pair.begin(); iter != sock_file_pair.end(); iter++)
						{
							if (iter->second == i)
								socket = iter->first; 
						}
						//if (socket == -1)
							//std::cout << "찾지 못함" << std::endl;
						//std::cout << "file 쓰기" << std::endl;
						int p = ft::ft_contain(sock_msg[socket], "\r\n\r\n");
						std::string last = sock_msg[socket];

						if (p != -1)
						{
							last =sock_msg[socket].substr(p, sock_msg[socket].size() - p);
						}
						write(file, last.c_str(), (last.size()));
						FD_CLR(file, &instance->write_set);
						close(file);
						//std::cout << "file 쓰기가 끝났습니다" << std::endl;

					}
				}
			}
		}
	}
}
