#ifndef BINDINGSOCKET_HPP
#define BINDINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE
{
	class BindingSocket : public SimpleSocket
 	{
		private:
			int binding;
			void connect_to_network(int sock, struct sockaddr_in address);

		public:
			BindingSocket(int domain, int service, int protocol, int port, u_long interface);// : SimpleSocket(domain, service, protocol, port, interface){}

			int get_binding();
	};
	
}

#endif