// main poll looper

#ifndef Webserv_HPP
#define Webserv_HPP

#include "Config.hpp"
#include <poll.h>

#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../Networking/Server/ServerListener.hpp"
#include "../Networking/Server/Server.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

namespace HDE
{
	using std::cerr;

	class Webserv
	{
		private:
			const CONF::Config	*configlist;

			typedef	std::vector<struct pollfd>		pollfd_vec;
			typedef std::map<int, Server *>			server_map;
			typedef std::map<int, ServerListener *>	sockfd_map;

			pollfd_vec fds;
			sockfd_map sockfds;
			server_map servers;

			void	add_sockfd(const CONF::ServerConfig *config);
			void	remove_server(int server_fd);
			void	add_server(ServerListener *sockfd);

		public:
			Webserv(const CONF::Config *config);
			~Webserv();

			void	run_servers(vector<ServerLocation> sl);
	};
} // namespace HDE

#endif
