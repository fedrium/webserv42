#include "Server.hpp"

void HDE::Server::handleGet(int socket)
{
	std::stringstream	response;

	if (handle_redirect(socket, header[1]))
		return;

	if ((header[1].find("/login") != string::npos || header[1].find("/register") != string::npos) && header[1].find(".html") == string::npos)
	{
	 	startLogin(socket);
		this->status = DONE;
		return;
	}

	if (this->status == ERROR_PENDING || this->status == ERROR)
	{
		cout << "[ERROR] An error has been detected. Aborting request..." << endl;
		this->status = ERROR;
		send_error_page(socket, "500");
		return;
	}

	if (this->extension == ".py")
	{
		cgi(socket);
		this->status = DONE;
		return;
	}

	if (access(file_path.c_str(), R_OK) != 0)
	{
		send_error_page(socket, "404");
		return;
	}

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
		this->status = DONE;
		send_whole(this->target_socket, response.str());
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
		return send_error_page(socket, "400");
	if (chopString(tmpInfo1[0], "=").size() != 2 || chopString(tmpInfo1[1], "=").size() != 2)
		return send_error_page(socket, "400");

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
	}
	else
		send_error_page(socket, "404");
}
