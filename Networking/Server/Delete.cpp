#include "Server.hpp"

void HDE::Server::handleDelete(int socket)
{
	string command, deletePath;
	deletePath = "./database/files" + header[1];
	if (access(deletePath.c_str(), 0) != 0)
	{
		std::cout << "Path not found!" << std::endl;
		return;
	}
	cout << "path: " << deletePath << std::endl;
	remove(deletePath.c_str());
	// close(socket);
}
