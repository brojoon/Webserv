#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <vector>


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

		const std::string &getLocationPath() const ;
		std::vector<std::string> &getAllowMethods() const;
		bool getAutoIndex() const;
		std::string getRoot() const;

		void setLocationName(std::string &Location_path);
		void setAllowMethods(std::string &allow_methods);
		void setAutoIndex(bool &autoindex);
		void setRoot(std::string root);
}