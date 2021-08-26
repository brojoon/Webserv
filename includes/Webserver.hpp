#ifndef Webserver_HPP
#define Webserver_HPP

#define WEBSERVER Webserver::getInstance()

class Webserver
{
	private:
		Webserver();

		static Webserver *instance;
	public:
		virtual ~Webserver();
		static Webserver* getInstance();
		
		bool parsing(char *str);
};


#endif