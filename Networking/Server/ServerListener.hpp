// idk what to name this
// listen to incoming connections, then create a Server object

#ifndef ServerListener_HPP
#define ServerListener_HPP

#include "../Sockets/hdelibcsockets.hpp"
#include "Server.hpp"
#include "../../srcs/Config.hpp"

namespace HDE
{
	class ServerListener
	{
		private:
			ListeningSocket				*socket;
			const	CONF::ServerConfig	*server_config;
			int							port;

		public:
			ServerListener(const CONF::ServerConfig *config, std::string port_str);
			~ServerListener();

			Server *accept_connection();
			int		get_socket();
			int		get_port();
			const CONF::ServerConfig *get_config() const;
	};

}

#endif