#ifndef CONNECTINGSOCKET_HPP
#define CONNECTINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE
{
	class ConnectingSocket : public SimpleSocket
 	{
		private:
			int	connecting;
			void connect_to_network(int sock, struct sockaddr_in address);

		public:
			ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);// : SimpleSocket(domain, service, protocol, port, interface){}

			int get_connecting();
	};
	
}

#endif