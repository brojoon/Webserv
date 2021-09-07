#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <vector>
#include <string>


class Location
{
	private:
		std::string location_path;
		std::string root;
		std::vector<std::string> limit_except;
		bool autoindex;
		std::string redirection;
		std::vector<std::string> default_files;
		std::map<std::string, std::string> cgi_path;

	public:
		Location();
		virtual ~Location();

		const std::string &getLocationPath() const;
		const std::string &getRoot() const;
		const bool &getAutoIndex() const;
		std::vector<std::string> &getLimitExcept();
		const std::string &getRedirection() const;
		std::vector<std::string> &getDifaultFiles();
		std::map<std::string, std::string> &getCgiPath();

		void setLocationPath(std::string &Location_path);
		void setAutoIndex(bool autoindex);
		void setRoot(std::string &root);
		void setRedirection(std::string &redirection);

};

#endif