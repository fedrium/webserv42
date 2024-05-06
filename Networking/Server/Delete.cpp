#include "Server.hpp"

void HDE::Server::handleDelete(int socket)
{
	string deletePath;
	std::stringstream response;
	deletePath = this->file_path;
	if (access(deletePath.c_str(), 0) != 0)
	{
		send_error_page(socket, "404");
		return;
	}
	else
		cout << "[INFO] Removing item located at " << deletePath << endl;
	remove(deletePath.c_str());

	response << "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\n";
    response << "<!DOCTYPE html>";
    response << "<html>";
    response << "<head>";
    response << "<title>Deletion done</title>";
    response << "<script>";
    response << "function redirectToPage() {";
    response << "window.location.href = \"/index.html\";";
    response << "}";
    response << "</script>";
    response << "</head>";
    response << "<body>";
    response << "<h2>Deletion done</h2>";
    response << "<body>";
    response << "<button onclick=\"redirectToPage()\">Go back</button>";
    response << "</body>";
    response << "</html>";

	send_whole(socket, response.str());
	this->status = DONE;
}
