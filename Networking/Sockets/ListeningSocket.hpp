#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"

namespace HDE
{
	class ListeningSocket : public BindingSocket
 	{
		private:
			int backlog;
			int listening;
		
		public:
			void connect_to_network(int sock, struct sockaddr_in address);
			ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg);

			void start_listening();

			int get_backlog();
			int get_listening();
	};
	
}

#endif