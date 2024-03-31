#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "srcs.hpp"
#include "ServerLocation.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;

class ServerLocation;

class ServerConfig
{
	private:
		std::vector<string>					ports;
		string								root;
		string								index;
		string								server_name;
		string								client_max;
		std::vector<string>					allowed_methods;
		std::map<string, string>			error_page;
		std::map<string, string>			cgi;
		std::map<string, ServerLocation>	locations;

	public:
		ServerConfig();
		~ServerConfig();

		void	parseInfo(vector<string> info);
		void	printInfo();

		void	set_ports(vector<string> info);
		void	set_root(vector<string> info);
		void	set_index(vector<string> info);
		void	set_server_name(vector<string> info);
		void	set_client_max(vector<string> info);
		void	set_allowed_methods(vector<string> info);
		void	set_error_page(vector<string> info);
		void	set_cgi(vector<string> info);
		void	set_locations(vector<string> info);


		// string	get_root();
		// string	get_index();
		// string	get_server_name();
		// string	get_client_max();
		// const std::vector<string>		&get_ports();
		// const std::map<string, string>	&get_error_page();
		// const std::map<string, string>	&get_cgi();
		// const std::vector<string>		&get_locations();
};

#endif