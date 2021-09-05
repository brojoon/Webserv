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
		std::vector<std::string> allow_methods;
		bool autoindex;
	
	public:
		Location();
		virtual ~Location();

		const std::string &getLocationPath() const;
		const std::string &getRoot() const;
		const bool &getAutoIndex() const;
		std::vector<std::string> &getAllowMethods();

		void setLocationPath(std::string &Location_path);
		void setAutoIndex(bool autoindex);
		void setRoot(std::string &root);

};

#endif