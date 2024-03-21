#include "SimpleSocket.hpp"

HDE::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);

    sock = socket(domain, service, protocol);
	// std::cout << sock << std::endl;
	test_connection(this->sock);
	// std::cout << sock << std::endl;
}

void HDE::SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
		// std::cout << item_to_test << std::endl;
        perror("Error");
		// std::cout << "Errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
		
    }
}

int HDE::SimpleSocket::get_sock()
{
    return (this->sock);
}

struct sockaddr_in HDE::SimpleSocket::get_address()
{
	return (this->address);
}
