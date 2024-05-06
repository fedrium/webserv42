#include "Server.hpp"

void HDE::Server::handlePost(int socket)
{
	if (this->status == ERROR_PENDING || this->status == ERROR)
	{
		cout << "[ERROR] An error has been detected. Aborting request..." << endl;
		send_error_page(socket, "413");
		this->status = DONE;
		return;
	}
	if (access(file_path.c_str(), R_OK) != 0)
	{
		send_error_page(socket, "404");
		return;
	}
	cgi(socket);
	this->status = DONE;
}
