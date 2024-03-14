#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace HDE
{
    class SimpleSocket
    {
        private:
            struct sockaddr_in address;
            int sock;
            int connection;

        public:
            SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
            virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
            void test_connection(int);

            int get_sock();
            int get_connection();
			struct sockaddr_in get_address();

			void set_connection(int con);
    };
}

#endif