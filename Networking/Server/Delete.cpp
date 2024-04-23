#include "Server.hpp"

void HDE::Server::handleDelete(int socket)
{
	string command, deletePath;
	deletePath = "./database/files" + header[1];
	command = "rm " + deletePath;
	system(command.c_str());
	close(socket);
}
