#include "Server.hpp"

void HDE::Server::handleGet(int socket)
{
	vector<string> validLocations;

	if (header[1] == "/")
		html(socket, "/index.html");
	else if (header[1].find(".html") != string::npos)
		html(socket, "");
	else if (header[1].find(".ico") != string::npos)
		ico(socket, "");
	else
		error(socket, "404");
}

void HDE::Server::error(int socket, string type)
{
	string returnClient, line, filePath;
	std::ifstream file;
	returnClient.append("HTTP/1.1 200 OK\r\n");
	returnClient.append("Content-Type: text/html\r\n\r\n");
	filePath.append("./public/error/").append(type).append(".html");
	file.open(filePath);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			returnClient.append(line);
		}
		int res = send(socket, returnClient.c_str(), returnClient.size(), 0);
		if (res < 0)
			perror("Can't send error file\n");
		file.close();
	}
	else
		perror("Can't open error file\n");
	close(socket);
}

void HDE::Server::html(int socket, string new_url)
{
	string returnClient, line, filePath;
	std::ifstream file;
	returnClient.append("HTTP/1.1 200 OK\r\n");
	returnClient.append("Content-Type: text/html\r\n\r\n");
	if (!new_url.empty())
		header[1] = new_url;
	filePath.append("./public/html").append(header[1]);
	file.open(filePath);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			returnClient.append(line);
		}
		int res = send(socket, returnClient.c_str(), returnClient.size(), 0);
		if (res < 0)
			perror("Can't send html file\n");
		file.close();
		close(socket);
	}
	else
		error(socket, "404");
	
}

void HDE::Server::png(int socket, string new_url)
{
	string returnClient, line, filePath;
	std::ifstream file;
	returnClient.append("HTTP/1.1 200 OK\r\n");
	returnClient.append("Content-Type: image/png\r\n\r\n");
	if (!new_url.empty())
		header[1] = new_url;
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

void HDE::Server::ico(int socket, string new_url)
{
	string returnClient, line, filePath;
	std::ifstream file;
	returnClient.append("HTTP/1.1 200 OK\r\n");
	returnClient.append("Content-Type: image/x-icon\r\n\r\n");
	if (!new_url.empty())
		header[1] = new_url;
	filePath.append("./public/ico").append(header[1]);
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
			perror("Can't send ico file\n");
	}
	else
		perror("Can't open ico file\n");
	file.close();
	close(socket);
}