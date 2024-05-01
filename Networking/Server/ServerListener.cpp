#include "ServerListener.hpp"

HDE::ServerListener::ServerListener(const CONF::ServerConfig *config, std::string port_str)
{
	port = std::atoi(port_str.c_str());

	this->server_config = new CONF::ServerConfig(*config);
	this->socket = new ListeningSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10);
}

HDE::ServerListener::~ServerListener()
{
}

HDE::Server	*HDE::ServerListener::accept_connection()
{
	int	client_fd;

	struct sockaddr_in address = socket->get_address();
	int addrlen = sizeof(address);
	client_fd = accept(socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (client_fd < 0)
	{
		cout << "[ERROR] Server failed to accept connection from" << socket->get_sock() << endl;
		return NULL;
	}

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		cout << "[ERROR] Failed to set socket to non-blocking, proceeding regardless..." << endl;

	return new Server(this->server_config, client_fd);
}

int	HDE::ServerListener::get_port()
{
	return (port);
}

int	HDE::ServerListener::get_socket()
{
	return socket->get_sock();
}

const CONF::ServerConfig *HDE::ServerListener::get_config() const
{
	return this->server_config;
}