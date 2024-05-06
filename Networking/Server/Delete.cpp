#include "Server.hpp"

void HDE::Server::handleDelete(int socket)
{
	string deletePath;
	deletePath = this->file_path;
	if (access(deletePath.c_str(), 0) != 0)
	{
		send_error_page(socket, "404");
		return;
	}
	else
		cout << "[INFO] Removing item located at " << deletePath << endl;
	remove(deletePath.c_str());
	this->status = DONE;
}
