#include "SimpleSocket.hpp"

HDE::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);
    connection = connect_to_network(sock, address);
}

void HDE::SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test)
    {
        perror("Error");
        return;
    }
}

int HDE::SimpleSocket::get_connection()
{
    return (connection);
}

int HDE::SimpleSocket::get_sock()
{
    return (sock);
}

struct sockaddr_in HDE::SimpleSocket::get_address()
{
	return (this->address);
}

void HDE::SimpleSocket::set_connection(int con)
{
	connection = con;
}
