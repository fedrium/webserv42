#ifndef SERVER_HPP
#define SERVER_HPP

#include "SimpleServer.hpp"
#include "../../srcs/srcs.hpp"

namespace CONF
{
	class Config;
}

using CONF::Config;

namespace HDE
{
	class Server : public SimpleServer
	{
		private:
			char buffer[30000] = {0};
			int new_socket;

			vector<string>		header;
			vector<string>		content;
			string			extension;

			void accepter();
			void handler();
			void responder();

		public:
			CONF::Config	*config;
			Server(CONF::Config	*config);

			// Server.cpp
			void launch();
			vector<string>	chopString(string str, string delimiter);

			// Get.cpp
			void handleGet(int socket);
			void errorPage (int socket);
			void html(int socket, string extension);
			void png(int socket, string extension);

			// Post.cpp
			void handlePost(int socket);
	};
}

#endif