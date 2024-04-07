#ifndef SERVER_HPP
#define SERVER_HPP

#include "SimpleServer.hpp"
#include "../../srcs/srcs.hpp"

namespace CONF
{
	class Config;
}

namespace HDE
{
	class Server : public SimpleServer
	{
		private:
			char buffer[30000] = {0};
			int new_socket;

			vector<string> header;

			void accepter();
			void handler();
			void responder();

		public:
			CONF::Config	*config;
			Server(CONF::Config	*config);
			void launch();

			vector<string>	chopString(string str, string delimiter);
	};
}

#endif