#include "Server.hpp"

void HDE::Server::handleGet(int socket)
{
	std::stringstream	response;

	if (handle_redirect(socket, header[1]))
		return;

	else if ((header[1].find("/login") != string::npos || header[1].find("/register") != string::npos) && header[1].find(".html") == string::npos)
	{
	 	startLogin(socket);
		this->status = DONE;
		return;
	}

	this->extension = extract_extension(header[1]);
	this->file_path = build_path(header[1]);

	if (access(file_path.c_str(), R_OK) != 0)
		this->file_path = "./public/error/404.html";

	response << "HTTP/1.1 200 OK\r\n";
	response << "Connection: keep-alive\r\n";
	response << "Content-Type: " << get_content_type(extension) << "\r\n";

	struct stat fileInfo;
	stat(file_path.c_str(), &fileInfo);
	if (fileInfo.st_size < BUFFER_SIZE) // no need chunking, send file in 1 go
	{
		string fileData;
		std::ifstream fileO;
		char fileBuffer[BUFFER_SIZE];

		fileO.open(file_path.c_str()); // read all file data into fileData
		if (fileO.is_open())
		{
			while (fileO.read(fileBuffer, sizeof(fileBuffer)))
				fileData.append(fileBuffer, sizeof(fileBuffer));
			fileData.append(fileBuffer, fileO.gcount()); // append remaining data that is in buffer
			fileO.close();
		}
		else
		{
			cout << "[ERROR] Error opening file" << endl;
			this->status = ERROR;
			return;
		}

		response << "Content-Length: " << fileInfo.st_size << "\r\n\r\n"; // internet says its mandatory bruh
		response << fileData; // add file data to stream
		send_whole(this->target_socket, response.str());
		this->status = DONE;
		return;
	}
	else // file too big, send in chunks
	{
		cout << "[INFO] File size too large, chunking will be done..." << endl;
		response << "Transfer-Encoding: chunked\r\n\r\n";
		send_whole(this->target_socket, response.str());
		this->fstream_for_chunk.open(file_path.c_str());
		if (!this->fstream_for_chunk.is_open())
		{
			cout << "[ERROR] Error opening file" << endl;
			this->status = ERROR;
			return;
		}
		this->status = SENDING_CHUNK;
		send_chunk();
		return;
	}
}

string HDE::Server::build_path(string url)
{
	string base, finalPath, root = "", index = "";
	vector<CONF::ServerLocation> locationVector = config->get_locations();
	int baseEnd, pathEnd;

	if (url.substr(1).find("/") != string::npos) // from /wow/wow.png, get [/wow]/wow.png
		baseEnd = url.substr(1).find("/");
	else
		baseEnd = url.substr(1).size();

	if (url.substr(1).find("?") != string::npos) // from /wow/wow.png?yea=yea, get [/wow/wow.png]?yea=yea
		pathEnd = url.substr(1).find("?");
	else
		pathEnd = url.substr(1).size();

	base = url.substr(0, baseEnd + 1);

	for (vector<CONF::ServerLocation>::iterator it = locationVector.begin(); it != locationVector.end(); it++)
	{
		if (it->get_path() == base)
		{
			root = it->get_root();
			if (root.empty())
				root = config->get_root();
			index = it->get_index();
			if (index.empty())
			{
				if (it->get_autoindex() == "on")
					this->autoindex = true;
				else
					index = config->get_index();
			}
			break;
		}
		else
		{
			root = config->get_root();
			index = config->get_index();
		}
	}

	if (root == config->get_root())
		finalPath = root + url.substr(0, pathEnd + 1);
	else
		finalPath = root + url.substr(baseEnd + 1, pathEnd + 1);

	struct stat path_stat;
	if ((!stat(finalPath.c_str(), &path_stat) && S_ISDIR(path_stat.st_mode)) || finalPath[finalPath.length() - 1] == '/') // check if path is folder
	{
		if (finalPath[finalPath.length() - 1] != '/')
			finalPath.append("/");
		if (this->autoindex == false)
		{
			if (index[0] == '/')
				finalPath = finalPath + index.substr(1);
			else
				finalPath = finalPath + index;
		}
	}

	if (finalPath[0] != '.')
		finalPath = "." + finalPath;
	cout << "[INFO] Final path To File: " << finalPath << endl;
	return finalPath;
}

int HDE::Server::handle_redirect(int socket, string url)
{
	for (size_t i = 0; i < config->get_locations().size(); i++)
	{
		if (config->get_locations()[i].get_path() == url)
		{
			this->redirect_url = config->get_locations()[i].get_return_url();
			if (redirect_url.empty())
				return (0);

			std::stringstream response;

			response << "HTTP/1.1 302 Found\r\n";
			response << "Location:" << this->redirect_url << "\r\n\r\n";
			cout << "[INFO] Redirecting to " << redirect_url << " right now..." << endl;
			send_whole(socket, response.str());
			this->status = DONE;
			return (1);
		}
	}
	return (0);
}

string HDE::Server::get_content_type(string extension)
{
	if (extension == ".html" || extension == ".css" || extension == ".plain")
		return ("text/" + extension.substr(1));
	else if (extension == ".png" || extension == ".jpeg" || extension == ".jpg")
		return ("image/" + extension.substr(1));
	else if (extension == ".mp4")
		return ("video/" + extension.substr(1));
	else if (extension == ".mp3")
		return ("audio/mpeg");
	else if (extension == ".ico")
		return ("image/x-icon");
	return ("");
}

void HDE::Server::send_chunk()
{
	char buffer[CHUNK_SIZE];
	string to_send;
	std::stringstream chunk, chunk_end;

	this->chunk_times += 1;

	if (!this->fstream_for_chunk.is_open())
	{
		cout << "[ERROR] Error opening file" << endl;
		this->status = ERROR;
		return;
	}
	if (this->fstream_for_chunk.read(buffer, CHUNK_SIZE))
	{
		to_send = string(buffer, sizeof(buffer));
		chunk << std::hex << to_send.length() << std::dec << "\r\n"; // each chunk should be sent preceded by its size
		chunk << to_send << "\r\n";
		cout << "[NOTICE] Sending chunk number " << chunk_times << " of size " << to_send.length() << "..." << endl;
		send_whole(this->target_socket, chunk.str());
	}
	else
	{
		to_send = string(buffer, this->fstream_for_chunk.gcount());
		chunk << std::hex << to_send.length() << std::dec << "\r\n"; // each chunk should be sent preceded by its size
		chunk << to_send << "\r\n";
		cout << "[NOTICE] Sending chunk number " << chunk_times << " of size " << to_send.length() << "..." << endl;
		this->fstream_for_chunk.close();
		send_whole(this->target_socket, chunk.str());
	
		chunk_end << std::hex << 0 << std::dec << "\r\n\r\n"; // send empty string to alert client that all data is sent
		send_whole(this->target_socket, chunk_end.str());
		cout << "[INFO] All data chunks have been sent" << endl;
		this->status = DONE;
	}
}

void HDE::Server::send_whole(int socket, string data)
{
	const char *dataCC = data.c_str();
	int bytesSent, dataLeft;
	dataLeft = data.size();

	while (dataLeft > 0)
	{
		bytesSent = send(socket, dataCC, dataLeft, 0); // send doesn't guarantee all data is sent wtf
		if (bytesSent == -1) // which apparently is a more frequent case with non-blocking sockets
		{
			std::cerr << "[ERROR] Error sending data to the client. Closing connection..." << endl;
			this->status = ERROR;
			return;
		}
		dataCC += bytesSent;
		dataLeft -= bytesSent;
	}
}

string HDE::Server::extract_extension(string url)
{
	int pathEnd;

	if (url.find("?") != string::npos)
		pathEnd = url.find("?");
	else
		pathEnd = url.size();

	if (url.find(".") != string::npos)
		return url.substr(this->header[1].find("."), pathEnd - url.find("."));
	else
		return "";
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
	file.open(filePath.c_str());
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
	file.open(filePath.c_str());
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

char*	dynamicDup(string s)
{
	char *str = new char[s.length() + 1];
	strcpy(str, s.c_str());
	return str;
}

void HDE::Server::cutstr(size_t pos, size_t size)
{
	this->content.erase(pos, size);
}

void HDE::Server::cgi(int socket)
{
	(void)socket;
	std::cout << "URL: " << this->header[1] << std::endl;
	std::string output;
	int status;

	char **args = new char*[3];
	args[0] = dynamicDup("/usr/bin/python3");
	args[1] = dynamicDup("./public/cgi/upload.py");
	args[2] = NULL;

	int readfd[2];
	int writefd[2];

	if (pipe(readfd) == -1 || pipe(writefd) == -1)
		return;
	int pid = fork();
	if (pid == 0)
	{
		dup2(readfd[1], STDOUT_FILENO);
		dup2(writefd[0], STDIN_FILENO);
		close(readfd[0]);
		close(readfd[1]);
		close(writefd[0]);
		close(writefd[1]);

		execve("/usr/bin/python3", args, 0);
		perror(strerror(errno));
		std::cerr << "execve failed" << std::endl;
		exit(1);
	}
	else
	{
		while (!this->content.empty())
		{
			if (this->content.length() > BUFFER_SIZE)
			{
				write(writefd[1], this->content.substr(0, BUFFER_SIZE).c_str(), BUFFER_SIZE);
				this->cutstr(0, BUFFER_SIZE);
			}
			else
			{
				write(writefd[1], this->content.c_str(), this->content.size());
				this->cutstr(0, this->content.size());
			}
		}
		close(writefd[1]);
		close(writefd[0]);
		close(readfd[0]);
		close(readfd[1]);
		waitpid(pid, &status, 0);
	}
}