#include "Server.hpp"

void HDE::Server::handleGet(int socket)
{
	vector<string> validLocations;

	if (header[1].find_last_of('.') != string::npos)
		extension = header[1].substr(header[1].find_last_of('.'), string::npos);
	if (extension == ".html")
		html(socket, extension);
	else
	{
		header[1] = "/index.html";
		html(socket, extension);
	}
}

void HDE::Server::html(int socket, string extension)
{
	string returnClient, line, filePath;
	std::ifstream file;
	returnClient.append("HTTP/1.1 200 OK\r\n");
	returnClient.append("Content-Type: text/html\r\n\r\n");
	filePath.append("./public/html").append(header[1]);
	file.open(filePath);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			returnClient.append(line);
		}
	}
	else
	{
		file.open("./public/error/404.html");
		while (!file.eof())
		{
			std::getline(file, line);
			returnClient.append(line);
		}
	}
	int res = send(socket, returnClient.c_str(), returnClient.size(), 0);
	if (res < 0)
		perror("Can't send html file\n");
	file.close();
	close(socket);
}

void HDE::Server::png(int socket, string extension)
{
	string returnClient, line, filePath;
	std::ifstream file;
	returnClient.append("HTTP/1.1 200 OK\r\n");
	returnClient.append("Content-Type: image/png\r\n\r\n");
	filePath.append("./public/png").append(header[1]);
	file.open(filePath);
	if (file.is_open())
	{
		char img_buffer[100000];
		while (file.read(img_buffer, sizeof(img_buffer)))
			returnClient.append(img_buffer, sizeof(img_buffer));
		if (file.eof())
			returnClient.append(img_buffer, file.gcount());
		int res = send(socket, returnClient.c_str(), returnClient.size(), 0);
		if (res < 0)
			perror("Can't send png file\n");
	}
	else
		perror("Can't open png file\n");
	file.close();
	close(socket);
}