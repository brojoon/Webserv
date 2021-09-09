#include "../includes/msg_checker.hpp"
#include "../includes/Server.hpp"

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
			//throw (std::string("404"));
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
		//throw (std::string("404"));
		return ;
	}
}

/*  잘못된 리다이 렉션
void msg_checker::find_redirect_url(Server& server, std::string& root, std::string redirect_Uri)
{
	const std::string url = info.url_abs_path;
	Location redirect_Location;
	std::map<std::string, Location> tem_map;
	std::vector<std::string> server_index = server.getDifaultFiles();

	tem_map = server.getLocations();

	if (tem_map.find(redirect_Uri) != tem_map.end())
	{
		
		redirect_Location = tem_map[redirect_Uri];
		if (redirect_Location.getRoot() != "")
		{
			root = redirect_Location.getRoot() + redirect_Uri;
		}
		else
			root = server.getRoot() + redirect_Uri;	
		info.status = "301";
		if (redirect_Location.getDifaultFiles().size() != 0)
			check_indexfile(root, redirect_Location.getDifaultFiles());
		else
			check_indexfile(root, server_index);
	}
	else
		info.status = "404";
}
*/

std::string	msg_checker::find_url(Server& server)
{
	typedef std::map<std::string, Location>::reverse_iterator iter;
	const std::string url = info.url_abs_path;
	std::string redirect_Uri;
	std::string root;
	int idx;
	std::vector<std::string> server_index = server.getDifaultFiles();
	
	iter start = server.getLocations().rbegin();
	iter end = server.getLocations().rend();
	for (iter i = start; i != end; i++)
	{
		std::string location = i->first;
		size_t len = location.size();
		if (url.compare(0, len, location) == 0)
		{
			if ( i->second.getLimitExcept().size() != 0)
			{	
				std::size_t j = 0;
				for (;j < i->second.getLimitExcept().size(); j++)
					if (i->second.getLimitExcept()[j] == info.method)
						break ;
				if (j == i->second.getLimitExcept().size())
				{		
					info.status = "403";
					//throw (std::string("404"));
				}			
			}
			if ((redirect_Uri = i->second.getRedirection()) != "")
			{
				root = redirect_Uri;
				info.status = "301";
				return (root);				
			}

			info.cgi_path = i->second.getCgiPath();	
			if (i->second.getRoot() != "")
				root = i->second.getRoot() + url;			
			else
				root = server.getRoot() + url;

			if (i->second.getDifaultFiles().size() != 0)
				check_indexfile(root, i->second.getDifaultFiles());
			else
				check_indexfile(root, server_index);
			if ((idx = root.find('.')) != std::string::npos)
				info.extention = root.substr(idx);
			if (info.cgi_path != "")
			{
				std::vector<std::string> cgi = i->second.getCigExcept();
				size_t i = 0;
				for (;i < cgi.size(); i++)
				{
					if (cgi[i] == info.extention)
					{
						info.is_cgi = true;
						break ;
					}
				}
				if (i == cgi.size())
					info.is_cgi = false;			
			}
			return (root);
		}
		location.clear();
	}
	root = server.getRoot() + url;
	check_indexfile(root, server_index);
	return (root);
}

msg_checker::return_type msg_checker::check(std::string &firstline, std::map<std::string, std::string> &map, int port)
{
	std::string tem;
	info.is_cgi = false;
	info.host = map["Host"];
	info.port = port;
	info.method = ft::ft_strtok(firstline, " ");
	std::string path = ft::ft_strtok(firstline, " ");
	std::string http = ft::ft_strtok(firstline, "/");
	info.version = ft::ft_strtok(firstline, "/");
	if (path.size() > 2048)
	{
		info.status = "414";
		return (info);
	}
	info.url_abs_path = ft::ft_strtok(path, "?");
	info.query = path;
	
	ft::split(map["Accept"], " ,", info.accept); // 값이 없으면 모든 미디어 유형 / 서버가 지원하지 않는경우 [406]
	ft::split(map["Accept-Language"], " ,", info.accept_Language);	
	ft::split(map["Accept-Encoding"], " ,", info.accept_Encoding); // 헤더에 따라 수용 가능한 응답을 보낼 수 없는 경우 서버는 [406(Not Acceptable)] 상태 코드와 함께 오류 응답
	info.user_Agent = map["User-Agent"]; // 여러 제품 토큰과 사용자 에이전트의 중요한 부분을 구성하는 에이전트 및 하위 제품을 식별하는 설명

	// 허용 method가 아닌경우 에러 메세지
	if (ft::isknown(info.method) == false)
	{
		info.status = "501";
		return (info);
	}
	else if (ft::isMethods(info.method) == false)
	{
		info.status = "405";
		return (info);
	}

	std::map<int, Server>::iterator server_iter = WEBSERVER->getServerList().begin();
	for (; server_iter != WEBSERVER->getServerList().end(); server_iter++)
	{
		if (server_iter->second.getServerName() == info.host)
		{
			std::vector<unsigned short>::iterator listen_iter = server_iter->second.getPorts().begin(); 
			for (; listen_iter != server_iter->second.getPorts().end(); listen_iter++)
			{
				if (*listen_iter == info.port)
				{
					info.error_pages = server_iter->second.getErrorPages();
					tem = find_url(server_iter->second);
					info.url_abs_path.clear();
					info.url_abs_path = tem;
					break ;
				}
			}
			if (listen_iter != server_iter->second.getPorts().end())
				break ;
		}
	}
	if (server_iter == WEBSERVER->getServerList().end())
	{
		server_iter = WEBSERVER->getServerList().begin();
		for (; server_iter != WEBSERVER->getServerList().end(); server_iter++)
		{
			std::vector<unsigned short>::iterator listen_iter = server_iter->second.getPorts().begin(); 
			for (; listen_iter != server_iter->second.getPorts().end(); listen_iter++)
			{
				if (*listen_iter == info.port)
				{
					info.error_pages = server_iter->second.getErrorPages();
					tem = find_url(server_iter->second);
					info.url_abs_path.clear();
					info.url_abs_path = tem;
					break ;
				}
			}
			if (listen_iter != server_iter->second.getPorts().end())
				break ;
		}
		if (server_iter == WEBSERVER->getServerList().end())
		{
			info.status = "400";
			return (info);
		}
	}
	std::cout << "root " << info.url_abs_path << std::endl;
	return (info);
}
