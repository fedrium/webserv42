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
	// parsebody();
	cgi(socket);
	this->status = DONE;
}
