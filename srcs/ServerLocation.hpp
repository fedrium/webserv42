#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include "srcs.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

class ServerLocation
{
	private:
		string					path;
		string					root;
		string					autoindex;
		string					index;
		std::vector<string>		methods;
		string					return_url;
		std::vector<string>		cgi_path;
		std::vector<string>		cgi_ext;
		string					client_max_body_size;

	public:
		ServerLocation();
		~ServerLocation();

		void set_path(std::vector<string> info);
		void set_root(std::vector<string> info);
		void set_methods(std::vector<string> info);
		void set_autoindex(std::vector<string> info);
		void set_index(std::vector<string> info);
		void set_return_url(std::vector<string> info);
		void set_cgi_path(std::vector<string> info);
		void set_cgi_extension(std::vector<string> info);
		void set_client_max_body_size(std::vector<string> info);
};

#endif