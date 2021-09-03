#include "../includes/msg_checker.hpp"

msg_checker::msg_checker()
{
	info.status = "200";
}

msg_checker::return_type msg_checker::check(std::string &firstline, std::map<std::string, std::string> &map)
{
 	info.method = ft::ft_strtok(firstline, " ");
	info.url_abs_path = ft::ft_strtok(firstline, " ");
	std::string http = ft::ft_strtok(firstline, "/");
	info.version = ft::ft_strtok(firstline, "/");

	std::string temp = ft::ft_strtok(info.url_abs_path, "?")
	info.query = temp
	if (ft::isknown(info.method) == false)
		info.status = "501";
	else if (ft::isMethods(info.method) == false)
		info.status = "405";
	
	std::string root;
	if (root +  )
	return return_type();
}
