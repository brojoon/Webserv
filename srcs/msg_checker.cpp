#include "../includes/msg_checker.hpp"
#include "../includes/Server.hpp"

msg_checker::msg_checker()
{
	info.status = "200";
}

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

std::string	msg_checker::find_url(Server& server)
{
	typedef std::map<std::string, Location>::reverse_iterator iter;
	const std::string url = info.url_abs_path;
	std::string redirect_Uri;
	std::string root;
	std::vector<std::string> server_index = server.getDifaultFiles();
	
	iter start = server.getLocations().rbegin();
	iter end = server.getLocations().rend();	
	for (iter i = start; i != end; i++)
	{
		std::string location = i->first;
		size_t len = location.size();
		if (url.compare(0, len, location) == 0)
		{
			if ((redirect_Uri = i->second.getRedirection()) != "")
			{
				find_redirect_url(server, root, redirect_Uri);
				return (root);					
			}
			if (i->second.getRoot() != "")
				root = i->second.getRoot() + url;			
			else
				root = server.getRoot() + url;	
			if (i->second.getDifaultFiles().size() != 0)
				check_indexfile(root, i->second.getDifaultFiles());
			else
				check_indexfile(root, server_index);
			return (root);
		}
		location.clear();
	}
	root = server.getRoot() + url;
	check_indexfile(root, server_index);
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

	// 값이 없으면 모든 미디어 유형 / 서버가 지원하지 않는경우 [406]
	ft::split(map["Accept"], " ,", info.accept);
	// 일단저장
	ft::split(map["Accept-Language"], " ,", info.accept_Language);
	// 헤더에 따라 수용 가능한 응답을 보낼 수 없는 경우 서버는 [406(Not Acceptable)] 상태 코드와 함께 오류 응답
	ft::split(map["Accept-Encoding"], " ,", info.accept_Encoding);
	// 여러 제품 토큰과 사용자 에이전트의 중요한 부분을 구성하는 에이전트 및 하위 제품을 식별하는 설명
	info.user_Agent = map["User-Agent"];

	// 값이 없으면 모든 언어 / 요청 응답 상관없이 내용의 언어 명시
	//예를 들어 한국 사람한테 일본어를 가르치는 사이트일 경우, 페이지 언어는 일본어더라도 Content-Language는 ko-KR일 수 있음
	ft::split(map["Content-Language"], " ,", info.content_Language);
	// 요청과 응답 메시지의 본문 크기를 바이트 단위로 표시
	info.content_Length = map["Content-Length"];
	// 그냥 파일 상대루트 반환하면 되는건가?
	info.content_Location = map["Content-Location"];
	// 컨텐츠의 타입(MIME)과 문자열 인코딩(utf-8 등등)을 명시할 수 있습니다. ex) Content-Type: text/html; charset=utf-8
	info.content_Type = map["Content-Type"];
	// ??
	info.last_Modified = map["Last-Modified"];

	// 허용 method가 아닌경우 에러 메세지
	if (ft::isknown(info.method) == false)
		info.status = "501";
	else if (ft::isMethods(info.method) == false)
		info.status = "405";

	std::map<int, Server>::iterator server_iter = WEBSERVER->getServerList().begin();
	for (; server_iter != WEBSERVER->getServerList().end(); server_iter++)
	{
		std::vector<unsigned short>::iterator listen_iter = server_iter->second.getPorts().begin(); 
		for (; listen_iter != server_iter->second.getPorts().end(); listen_iter++)
		{
			if (*listen_iter == i_port)
			{
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
		info.status = "400";

	std::cout << "root " << info.url_abs_path << std::endl;
	return (info);
}
