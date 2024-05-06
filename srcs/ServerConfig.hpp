#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "srcs.hpp"
#include "ServerLocation.hpp"

class ServerLocation;

namespace CONF
{
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
			std::vector<ServerLocation>			locations;

			int									count;
			int									Parent;

		public:
			ServerConfig();
			ServerConfig(const CONF::ServerConfig &obj);
			~ServerConfig();

			void	parseInfo(vector<string> info);
			void	printInfo() const;

			void	set_server_attr(vector<string> info);

			void	set_ports(vector<string> info);
			void	set_root(vector<string> info);
			void	set_index(vector<string> info);
			void	set_server_name(vector<string> info);
			void	set_client_max(vector<string> info);
			void	set_allowed_methods(vector<string> info);
			void	set_error_page(vector<string> info);
			void	set_cgi(vector<string> info);
			void	set_locations(vector<string> info, bool nested);

			const string	get_root() const;
			const string	get_index() const;
			const string	get_server_name() const;
			const string	get_client_max() const;
			const std::vector<string>				get_ports() const;
			const std::vector<string>				get_allowed_methods() const;
			const std::map<string, string>		get_error_page() const;
			const std::map<string, string>		get_cgi() const;
			const std::vector<ServerLocation>		get_locations() const;

			void	set_parent(int i);
	};
};

#endif
