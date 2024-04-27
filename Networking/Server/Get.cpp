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
	else if (header[1].find("/login") != string::npos || header[1].find("/register") != string::npos)
		startLogin(socket);
	else
		error(socket, "404");
}

void HDE::Server::startLogin(int socket)
{
	string userInput, userUsername, userPassword;

	userInput = chopString(header[1], "?")[1];
	vector<string> tmpInfo1 = chopString(userInput, "&");

	if (tmpInfo1.size() < 3)
		return error(socket, "400");
	if (chopString(tmpInfo1[0], "=").size() != 2 || chopString(tmpInfo1[1], "=").size() != 2)
		return error(socket, "400");

	userUsername = chopString(tmpInfo1[0], "=")[1];
	userPassword = chopString(tmpInfo1[1], "=")[1];

	if (header[1].find("/login") != string::npos)
		doLogin(socket, userUsername, userPassword);
	else if (header[1].find("/register") != string::npos)
		doRegister(socket, userUsername, userPassword);
}

void HDE::Server::doLogin(int socket, string uname, string pwd)
{
	std::ifstream	csvFile("./database/login.csv");
	string line, formatToCompare;
	bool validLogin = 0;

	formatToCompare = uname + "," + pwd;
	while (std::getline(csvFile, line))
	{
		if (formatToCompare == line)
			validLogin = 1;
	}
	if (validLogin)
		html(socket, "/upload.html");
	else
		html(socket, "/login_fail.html");
}

void HDE::Server::doRegister(int socket, string uname, string pwd)
{
	std::ofstream csvFile;
	std::ifstream cmpCsvFile;
	string line, unameRegged;
	cmpCsvFile.open("./database/login.csv");
	while (std::getline(cmpCsvFile, line))
	{
		unameRegged = chopString(line, ",")[0];
		if (uname == unameRegged)
		{
			html(socket, "/acc_exists.html");
			cmpCsvFile.close();
			return;
		}
	}
	cmpCsvFile.close();
    csvFile.open("./database/login.csv", std::ios::out | std::ios::app);
    csvFile << uname << "," << pwd << endl;
	csvFile.close();
	html(socket, "/reg_success.html");
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
	// close(socket);
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
		// close(socket);
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
	// close(socket);
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
		file.close();	
	}
	else
		perror("Can't open ico file\n");
	// close(socket);
}

void HDE::Server::cgi(int socket)
{
	std::cout << "URL: " << this->header[1] << std::endl;
	std::string output;
	int bytes_read;
	char *var[] = {(char *)header[1].c_str(), NULL};
	int stdout_fd = dup(1), stdin_fd = dup(0);

	output.append("HTTP/1.1 200 OK\r\n");
	output.append("Content-Type: text/html\r\n\r\n");

	int fd[2];

	if (pipe(fd) == -1)
		return;
	int pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		execve("./public/cgi/upload.py", var, 0);
		perror(strerror(errno));
		std::cerr << "execve failed" << std::endl;
		exit(1);
	}
	else
	{
		close(fd[1]);
		waitpid(pid, NULL, 0);
		char buffer[1000];
		ssize_t bytes_read;
		while ((bytes_read = read(fd[0], buffer, 1000)) > 0)
			output.append(buffer, bytes_read);
		std::cout << output;
		close(fd[0]);
	}
	dup2(stdout_fd, 1);
	dup2(stdin_fd, 0);
	send(socket, output.c_str(), output.size(), 0);
	// close(socket);
}