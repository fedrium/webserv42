#include "Webserv.hpp"

HDE::Webserv::Webserv(const CONF::Config *config)
{
	this->configlist = config;

	cout << "[INIT] Beginning initilization of webserver" << endl;
	cout << "[INIT] Each socket is represented by a fd" << endl;
	cout << endl;

	const vector<CONF::ServerConfig> &config_server = configlist->getServers();
	for (vector<CONF::ServerConfig>::const_iterator it = config_server.begin(); it != config_server.end(); ++it)
		add_sockfd(&(*it));
}

HDE::Webserv::~Webserv()
{
}

void	HDE::Webserv::add_sockfd(const	CONF::ServerConfig *config)
{
	vector<string>	port_list = config->get_ports();
	ServerListener	*new_sockfd;
	int	fd;

	for (vector<string>::iterator it = port_list.begin(); it != port_list.end(); ++it)
	{
		pollfd	sockfd_poll;
		new_sockfd = new ServerListener(config, (*it));
		fd = new_sockfd->get_socket();

		sockfd_poll.events = POLLIN; // server always listens first
		sockfd_poll.fd = fd;
		fds.push_back(sockfd_poll);

		sockfds.insert(std::pair<int, ServerListener *>(fd, new_sockfd)); // add to map
		cout << "[INIT] Server will now listen from fd " << fd << endl;
	}
	cout << "[INIT] Server will now listen to client ports ";
	for (vector<string>::iterator it = port_list.begin(); it != port_list.end(); ++it)
		cout << (*it) << " ";
	cout << endl;
}

void	HDE::Webserv::run_servers()
{
	size_t	total_fds;
	Server	*current; // current server to run

	while (true)
	{
		total_fds = sockfds.size() + servers.size();

		cout << endl << "=============================================" << endl;
		cout << "[INFO] Number of servers active      : " << servers.size() << endl;
		cout << "[INFO] Ports in use                  : ";
		for (sockfd_map::iterator it = sockfds.begin(); it != sockfds.end(); ++it)
			cout << it->second->get_port() << "  ";
		cout << endl;

		if (poll(&fds[0], total_fds, 10*60*1000))
		{
			for (size_t i = 0; i < total_fds; ++i)
			{
				if (fds[i].revents == 0) // no events on any fd
					continue;

				if (i < sockfds.size()) // for new connection to server
				{
					if (fds[i].revents & POLLIN) // there is data to read (without blocking)
					{
						ServerListener	*newconn = sockfds[fds[i].fd];
						cout << "[NOTICE] Establishing new connection... " << endl;
						add_server(newconn);
					}
				}
				else // for connected clients
				{
					if (fds[i].revents & POLLIN) // there is data to read (without blocking)
					{
						cout << "[NOTICE] Receiving data from fd " << fds[i].fd << endl;
						current = servers[fds[i].fd];
						if (!current->accepter()) // if read returns zero, socket disconnected, so remove the server
							remove_server(fds[i].fd);
						else
							fds[i].events = POLLOUT; // finish reading, now write instead
					}
					else if (fds[i].revents & POLLOUT) // writing is now possible
					{
						cout << "[NOTICE] Sending data through fd " << fds[i].fd << endl;
						current->handler();
						current->responder();

						if (current->get_status() == SENDING_CHUNK)
							i--;
						if (current->get_status() == DONE || current->get_status() == ERROR)
							remove_server(fds[i].fd);
					}
					else
					{
						cout << "[ERROR] Error at server fd " << fds[i].fd << endl;
						if (fds[i].revents & POLLHUP) // client closed connection
							cout << "[INFO] Client is no longer connected" << endl;
						remove_server(fds[i].fd);
					}
				}
			}
		}
	}
}

void	HDE::Webserv::remove_server(int server_fd)
{
	cout << "[NOTICE] Closing connection to server of fd " << server_fd << endl;
	close(server_fd);

	for (pollfd_vec::iterator rm_fd = fds.begin(); rm_fd != fds.end(); ++rm_fd)
	{
		if ((*rm_fd).fd == server_fd)
		{
			fds.erase(rm_fd); // remove from polling fd list
			// delete this->servers[server_fd]->get_config();
			break;
		}
	}

	// delete this->servers[server_fd];
	this->servers.erase(server_fd); // remove from map
	cout << "[NOTICE] Removed server of fd " << server_fd << endl;
}

void	HDE::Webserv::add_server(ServerListener *sockfd)
{
	Server *new_server = sockfd->accept_connection();
	int	fd = new_server->get_socket();
	pollfd	client_poll;

	client_poll.events = POLLIN;
	client_poll.fd = fd;
	fds.push_back(client_poll);

	servers.insert(std::pair<int, Server *>(fd, new_server)); // add to map
	cout << "[NOTICE] Added server with fd " << fd << endl;
}
