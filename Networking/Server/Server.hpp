#ifndef SERVER_HPP
#define SERVER_HPP

#include "SimpleServer.hpp"

namespace HDE
{
	class Server : public SimpleServer
	{
		private:
			char buffer[30000] = {0};
			int new_socket;

			void accepter();
			void handler();
			void responder();

		public:
			Server();
			void launch();
	};
}

#endif