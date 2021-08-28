#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <vector>


class Location
{
	private:
		std::string location_name;
		std::vector<std::string> allow_methods;
		bool autoindex;
	
	public:
		Location();
		virtual ~Location();

		const std::string &getLocationName() const ;
		std::vector<std::string> &getAllowMethods() const;
		bool getAutoIndex() const ;

		void setLocationName(std::string &Location_name);
		void setAllowMethods(std::string &allow_methods);
		void setAutoIndex(bool &autoindex);
}