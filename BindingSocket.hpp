#ifndef BINDINGSOCKET_HPP
#define BINDINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE
{
	class BindingSocket : public SimpleSocket
 	{
		public:
			int connect_to_network(int sock, struct sockaddr_in address);
			BindingSocket(int domain, int service, int protocol, int port, u_long interface);// : SimpleSocket(domain, service, protocol, port, interface){}

	};
	
}

#endif