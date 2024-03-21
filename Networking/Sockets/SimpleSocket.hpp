#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <cerrno>

namespace HDE
{
    class SimpleSocket
    {
        private:
            struct sockaddr_in address;
            int sock;

        public:
            SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
            virtual void connect_to_network(int sock, struct sockaddr_in address) = 0;
            void test_connection(int);

            int get_sock();
			struct sockaddr_in get_address();
    };
}

#endif