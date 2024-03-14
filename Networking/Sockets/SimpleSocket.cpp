#include "SimpleSocket.hpp"

HDE::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);

	const int enable = 1;
	if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;

	// set socket to non-blocking
	if (fcntl(this->sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		std::cerr << "failed to set socket to non-blocking" << std::endl;

	test_connection(this->sock);
}

void HDE::SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test)
    {
        perror("Error");
        return;
    }
}

int HDE::SimpleSocket::get_sock()
{
    return (sock);
}

struct sockaddr_in HDE::SimpleSocket::get_address()
{
	return (this->address);
}
