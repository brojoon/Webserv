#include "../includes/util_err.hpp"

namespace ft
{
	err::err()
	{
		_err["100"] = "Continue";
		_err["101"] = "Switching protocols";

		_err["200"] = "OK";
		_err["201"] = "Created";
		_err["202"] = "Accepted";
		_err["203"] = "Non-authoritative information";
		_err["204"] = "No content";

		_err["301"] = "Moved permanently";
		_err["302"] = "Not temporarily";
		_err["304"] = "Not modified";

		_err["400"] = "Bad Request";
		_err["401"] = "Unauthorized";
		_err["402"] = "Payment required";
		_err["403"] = "Forbidden";
		_err["404"] = "Not found";
		_err["405"] = "Method not allowed";
		_err["407"] = "Porxy authentication required";
		_err["408"] = "Request timeout";
		_err["410"] = "Gone";
		_err["412"] = "Precondition failed";
		_err["413"] = "Request Entity Too Large";
		_err["414"] = "Request-URI too long";

		_err["500"] = "Internal server error";
		_err["501"] = "Not implemented";
		_err["503"] = "Service unnailable";
		_err["504"] = "Gateway timeout";
		_err["505"] = "HTTP version not supported";
		_err403 = "<html><head><title>403 Forbidden</title></head><body><center><h1>403 Forbidden</h1></center><hr><center>42Webserv/1.0</center></body></html>";
		_err404 = "<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr><center>42Webserv/1.0</center></body></html>";
	}
	err::~err(){}
	std::string err::get_err(std::string status)
	{
		if (_err.count(status))
			return _err[status];
		return ("");
	}
	std::string err::get403()
	{
		return (this->_err403);
	}
	std::string err::get404()
	{
		return (this->_err404);
	}
}