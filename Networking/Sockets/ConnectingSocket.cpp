#include "ConnectingSocket.hpp"

HDE::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : 
	SimpleSocket(domain, service, protocol, port, interface)
{
	connect_to_network(get_sock(), get_address());
	test_connection(connecting);
}

void HDE::ConnectingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	connecting = connect(sock, (struct sockaddr *)&address, sizeof(address));
}

int HDE::ConnectingSocket::get_connecting()
{
	return (connecting);
}
