#include "Server.hpp"

void HDE::Server::handleGet(int socket)
{
	vector<string> validLocations;

	for (int i = 0; i < config->getServers()[0].get_locations().size(); i++)
	{
		if (header[1] != config->getServers()[0].get_locations()[i].get_path());
			return (errorPage(socket));
	}
	extension = header[1].substr(header[1].find_last_of('.'), string::npos);
	if (header[2] == "/poo")
		poo(socket, extension);
}

void HDE::Server::poo(int socket, string extension)
{
	string htmlReturn = "HTTP/1.1 200 OK\r\n";
	std::ifstream file("../../public/html/poo.html");
	string line;
	while (std::getline(file, line))
	{
		htmlReturn.append(line);
		htmlReturn.append("\r\n");
	}
	send(socket, htmlReturn.c_str(), sizeof(htmlReturn.c_str()), 0);
	close(socket);
}