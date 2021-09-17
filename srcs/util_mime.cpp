#include "../includes/util_mime.hpp"

namespace ft
{
	mime::mime()
	{
		_mime[".aac"] = "audio/aac";
		_mime[".abw"] = "application/x-abiword";
		_mime[".arc"] = "application/octet-stream";
		_mime[".atom"] = "application/atom+xml";
		_mime[".avi"] = "video/x-msvideo";
		_mime[".azw"] = "application/vnd.amazon.ebook";
		_mime[".bin"] = "application/octet-stream";
		_mime[".bmp"] = "image/x-ms-bmp";
		_mime[".bz"] = "application/x-bzip";
		_mime[".bz2"] = "application/x-bzip2";
		_mime[".csh"] = "application/x-csh";
		_mime[".css"] = "text/css";
		_mime[".csv"] = "text/csv";
		_mime[".doc"] = "application/msword";
		_mime[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
		_mime[".epub"] = "application/epub+zip";
		_mime[".gif"] = "image/gif";
		_mime[".htm"] = "text/html";
		_mime[".html"] = "text/html";
		_mime[".ico"] = "image/x-icon";
		_mime[".ics"] = "text/calendar";
		_mime[".jar"] = "application/java-archive";
		_mime[".jpeg"] = "image/jpeg";
		_mime[".jpg"] = "image/jpeg";
		_mime[".js"] = "application/js";
		_mime[".json"] = "application/json";
		_mime[".mid"] = "audio/midi";
		_mime[".midi"] = "audio/midi";
		_mime[".mov"] = "video/quicktime";
		_mime[".mpeg"] = "video/mpeg";
		_mime[".mpkg"] = "application/vnd.apple.installer+xml";
		_mime[".mp3"] = "audio/mpeg";
		_mime[".mp4"] = "video/mp4";
		_mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
		_mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
		_mime[".odt"] = "application/vnd.oasis.opendocument.text";
		_mime[".oga"] = "audio/ogg";
		_mime[".ogg"] = "audio/ogg";
		_mime[".ogv"] = "video/ogg";	
		_mime[".ogx"] = "application/ogg";
		_mime[".pdf"] = "application/pdf";
		_mime[".php"] = "text/html";
		_mime[".png"] = "image/png";
		_mime[".ppt"] = "application/vnd.ms-powerpoint";
		_mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
		_mime[".rar"] = "application/x-rar-compressed";
		_mime[".rtf"] = "application/rtf";
		_mime[".sh"] = "application/x-sh";
		_mime[".svg"] = "image/svg+xml";
		_mime[".svgz"] = "image/svg+xml";
		_mime[".swf"] = "application/x-shockwave-flash";
		_mime[".tar"] = "application/x-tar";
		_mime[".tif"] = "image/tiff";
		_mime[".tiff"] = "image/tiff";
		_mime[".ttf"] = "application/x-font-ttf";
		_mime[".txt"] = "text/plain";
		_mime[".vsd"] = "application/vnd.visio";
		_mime[".war"] = "application/java-archive";
		_mime[".wav"] = "audio/x-wav";
		_mime[".wave"] = "audio/wave";
		_mime[".weba"] = "audio/webm";
		_mime[".webm"] = "video/webm";
		_mime[".webp"] = "image/webp";
		_mime[".woff"] = "font/woff";
		_mime[".woff2"] = "font/woff2";
		_mime[".wmv"] = "video/x-ms-wmv";
		_mime[".xhtml"] = "application/xhtml+xml";
		_mime[".xls"] = "application/vnd.ms-excel";
		_mime[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
		_mime[".xml"] = "application/xml";
		_mime[".xul"] = "xul";
		_mime[".zip"] = "application/zip";
		_mime[".3gpp"] = "video/3gpp";
		_mime[".3gp"] = "video/3gpp";
		_mime[".3g2"] = "video/3gpp2";
		_mime[".7z"] = "application/x-7z-compressed";	
	}
	mime::~mime(){}
	std::string mime::get_mime_type(std::string extension)
	{
		if (_mime.count(extension))
			return _mime[extension];
		return ("application/octet-stream");
	}
}
