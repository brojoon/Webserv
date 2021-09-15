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

std::map<int, client> &Webserver::getClientList()
{
	return this->client_list;
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
	//int select_count = 1;
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
	//std::cout << "fd_max : " << fd_max << std::endl;

	while(1)
	{
		//timeout.tv_sec = 5;
		//timeout.tv_usec = 500;
		instance->read_temp = instance->read_set;
		instance->write_temp = instance->write_set;
		//std::cout << "select count: " << select_count++ <<  std::endl;
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
							//std::cout << "new clnt is connected" << std::endl;
							clnt_adr_size = sizeof(clnt_adr);
							clnt_sock = accept(instance->server_sockets[it->first].socket, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
							if (clnt_sock == -1)
								error_handling("accept() error");
							//std::cout << "clnt sock  " << clnt_sock << std::endl;
							instance->client_sockets.insert(std::pair<int, unsigned short>(clnt_sock, it->second.serv_adr.sin_port));
							instance->is_socket_end[clnt_sock] = false;
							//std::cout << "accept_clnt_sock: " << clnt_sock << std::endl;
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
						//std::cout << "port : " << port << std::endl;
						client_list[i].request(i, port);
						if (client_list[i].isReadEnd() == true)
							continue;
						std::string t = client_list[i].get_response();
						if (t.empty())
							continue;
						std::string t1 = client_list[i].get_body();
						response_list[i] = t1;
						FD_SET(i, &instance->write_set);
					}
				}
				
				if (FD_ISSET(i, &instance->write_temp) && !FD_ISSET(i, &instance->read_temp)) //write 
				{
					std::cout << "client socket write : " << i << std::endl;
					if (write(i, response_list[i].c_str(), response_list[i].size()) <= 0)
					{
						FD_CLR(i, &instance->write_set);
						FD_CLR(i, &instance->read_set);
						close(i);
						instance->client_sockets.erase(i);
					}
					else
					{
						if (instance->is_socket_end[i] == true)
						{
							FD_CLR(i, &instance->write_set);
							close(i);
							instance->client_sockets.erase(i);
						}
						else
							FD_CLR(i, &instance->write_set);
					}
					client_list.erase(i);
					response_list[i].clear();
				}
			}
		}
	}
}
