#include "../includes/msg_checker.hpp"
#include "../includes/Server.hpp"

msg_checker::msg_checker()
{
	info.status = "200";
}

//경로만 오면 인덱스 붙여주고 경로에 파일이 없으면 에러  (벡터부분 추가해야함)
void check_indexfile(msg_checker::return_type& info, std::string& root)
{
	std::string tem = root;
	std::string tem_index;
	std::vector<std::string> v;
	v.push_back("index.html");
	v.push_back("test.html");
	
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
		//index가 있는지 없지지 if문 만들고 그안에 넣기
		for (std::size_t i = 0;  i != v.size(); i++)
		{
			tem_index = tem + v[i];
			tem_index = "." + tem_index;
			if (access(tem_index.c_str(), 0) == 0)
			{
				root.clear();
				root = tem_index;		
				return ;			
			}
			tem_index.clear();
		}
		info.status = "404";
		return;
	}
}

void find_redirect_url(msg_checker::return_type& info, std::string& root, std::string redirect_Uri)
{
	const std::string url = info.url_abs_path;
	Location redirect_Location;
	std::map<std::string, Location> tem_map;
	const int i_port = atoi(info.port.c_str());

	tem_map = WEBSERVER->getServerList()[i_port].getLocations();
	if (tem_map.find(redirect_Uri) != tem_map.end())
	{
		redirect_Location = tem_map[redirect_Uri];
		if (redirect_Location.getRoot() != "")
			root = redirect_Location.getRoot() + url;			
		else
			root = WEBSERVER->getServerList()[i_port].getRoot() + url;	
		info.status = "301";
		check_indexfile(info, root);
	}
	else
		info.status = "404";
}

std::string	find_url(msg_checker::return_type& info)
{
	typedef std::map<std::string, Location>::reverse_iterator iter;
	const std::string url = info.url_abs_path;
	std::string redirect_Uri;
	const int i_port = atoi(info.port.c_str());
	std::string root;
	
	
	iter start = WEBSERVER->getServerList()[i_port].getLocations().rbegin();
	iter end = WEBSERVER->getServerList()[i_port].getLocations().rend();	
	for (iter i = start; i != end; i++)
	{
		std::string location = i->first;
		size_t len = location.size();
		if (url.compare(0, len, location) == 0)
		{
			if (false)
			//if ((redirect_Uri = i->second.getRedirection()) != "")
			{
				find_redirect_url(info, root, redirect_Uri);
				return (root);					
			}
			if (i->second.getRoot() != "")
				root = i->second.getRoot() + url;			
			else
				root = WEBSERVER->getServerList()[i_port].getRoot() + url;	
			check_indexfile(info, root);
			return (root);
		}
		location.clear();
	}
	root = WEBSERVER->getServerList()[i_port].getRoot() + url;
	check_indexfile(info, root);
	return (root);
}

msg_checker::return_type msg_checker::check(std::string &firstline, std::map<std::string, std::string> &map)
{
	std::string tem;
	info.method = ft::ft_strtok(firstline, " ");
	std::string path = ft::ft_strtok(firstline, " ");
	std::string http = ft::ft_strtok(firstline, "/");
	info.version = ft::ft_strtok(firstline, "/");
	info.url_abs_path = ft::ft_strtok(path, "?");
	info.query = path;
	
	info.ip = ft::ft_strtok(map["Host"], ":");
	info.port = map["Host"];
	const int i_port = atoi(info.port.c_str());

	if (ft::isknown(info.method) == false)
		info.status = "501";
	else if (ft::isMethods(info.method) == false)
		info.status = "405";

	if (WEBSERVER->getServerList().find(i_port) != (WEBSERVER->getServerList().end()))
	{
		tem = find_url(info);
		info.url_abs_path.clear();
		info.url_abs_path = tem;
	}
	else
	{
		info.status = "400";
	}
	//std::cout << "root " << info.url_abs_path << std::endl;
	return return_type();
}
