#ifndef CONNECTINGSOCKET_HPP
#define CONNECTINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE
{
	class ConnectingSocket : public SimpleSocket
 	{
		public:
			int connect_to_network(int sock, struct sockaddr_in address);
			ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);// : SimpleSocket(domain, service, protocol, port, interface){}

	};
	
}

#endif