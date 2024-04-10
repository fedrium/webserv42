#include "Server.hpp"

void HDE::Server::handleGet(int socket)
{
	vector<string> validLocations;

	// for (int i = 0; i < config->getServers()[0].get_locations().size(); i++)
	// {
	// 	if (header[1] != config->getServers()[0].get_locations()[i].get_path());
	// 		return (errorPage(socket));
	// }
	// extension = header[1].substr(header[1].find_last_of('.'), string::npos); //error
	if (header[1] == "/poo")
		poo(socket, extension);
	else
	{
		char *text = (char *)"I will fking piss white cement it will gush out of my pipe Oo la la mamamia";
		write(new_socket, text, strlen(text));
		close(new_socket);
	}
}

void HDE::Server::poo(int socket, string extension)
{
	string htmlReturn, line;
	std::ifstream file;
	htmlReturn.append("HTTP/1.1 200 OK\r\n");
	htmlReturn.append("Content-Type: text/html\r\n\r\n");
	file.open("./public/html/poo.html");
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			htmlReturn.append(line);
		}
		int res = send(socket, htmlReturn.c_str(), htmlReturn.size(), 0);
		if (res < 0)
			perror("Can't send html file\n");
		file.close();
	}
	else
		perror("Can't open html file\n");
	close(socket);
}