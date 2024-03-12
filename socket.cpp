#include "webserv.hpp"

HDE::simple_socket::simple_socket(int domain, int service, int protocol, int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, service, protocol);
    connection = connect_to_network(sock, address);
}

void HDE::simple_socket::test_connection(int item_to_test)
{
    if (item_to_test)
    {
        perror("Error");
        return;
    }
}

int HDE::simple_socket::get_connection()
{
    return (connection);
}

int HDE::simple_socket::get_sock()
{
    return (sock);
}