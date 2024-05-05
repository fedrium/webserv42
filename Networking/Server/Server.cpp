#include "Server.hpp"

HDE::Server::Server(const CONF::ServerConfig *config, int client_fd)
{
	this->target_socket = client_fd;
	this->config = config;
	this->status = NEW;
	this->chunk_times = 0;
	this->autoindex = false;
}

HDE::Server::~Server()
{
}

int HDE::Server::accepter()
{
	string request;
	char buffer[BUFFER_SIZE];
	int bytesRead;

	if (this->status != NEW)
		return (1);

	while ((bytesRead = read(this->target_socket, buffer, sizeof(buffer))) > 0)
	{
		request.append(buffer, bytesRead);
		size_t endOfHeader = request.find("\r\n\r\n");
		if (endOfHeader != string::npos)
		{
			headers = request.substr(0, endOfHeader + 4); // is entire header
			content = request.substr(endOfHeader + 4); // is everything after entire header
			header.clear(); // is general header
			for (int i = 0; i < 3; i++)
				header.push_back(chopString(headers, " \r\n")[i]);

			size_t contentLengthPos = headers.find("Content-Length: ");
			if (contentLengthPos != std::string::npos) // if Content-Length header is present
			{
				int beforeLenValuePos = contentLengthPos + string("Content-Length: ").length(); // e.g. Content-Length : (here)123
				int afterLenValuePos = headers.find("\r\n", beforeLenValuePos); // e.g. Content-Length : 123(here)

				size_t contentLength = atoi((headers.substr(beforeLenValuePos, afterLenValuePos - beforeLenValuePos)).c_str()); // get content length as int from string
				if (get_client_max_server() && contentLength > (size_t)get_client_max_server())
				{
					this->status = ERROR_PENDING;
					return headers.length();
				}
				while (content.size() < contentLength) // continue reading until specified length
				{
					if ((bytesRead = read(this->target_socket, buffer, sizeof(buffer))) > 0)
						content.append(buffer, bytesRead);
				}
			}
			break;
		}
	}
	return headers.length() + content.length();
}

int HDE::Server::get_client_max_server()
{
	if (config->get_client_max().empty() || config->get_client_max() == "0")
		return (0);

	string size, value_str, suffix;
	size = config->get_client_max();
	value_str = size.substr(0, size.find_first_not_of("0123456789"));
	suffix = size.substr(size.find_first_not_of("0123456789"));
	int value = atoi(value_str.c_str());

	if (suffix == "MB" || suffix == "M")
		value *= 1000000;
	else if (suffix == "KB" || suffix == "K")
		value *= 1000;
	else if (suffix == "B")
		value *= 1;
	else
	{
		cout << "[ERROR] client_max_body_size for server has an unexpected value. Ignoring it..." << endl;
		return (0);
	}

	return (value);
}

void HDE::Server::handler()
{
	switch (this->status)
	{
		case NEW:
			cout << "[INFO] General Header Received: ----------------------" << endl;
			for (size_t i = 0; i < header.size(); i++)
				cout << header[i] << " ";
			cout << endl;
			// cout << "[INFO] Header Received: ------------------------------" << endl;
			// cout << headers << endl;
			// cout << "[INFO] Content Received: -----------------------------" << endl;
			// cout << content << endl;
			// cout << endl;
			break;
		case SENDING_CHUNK:
			break;
		case DONE:
			break;
		default:
			break;
	}
}

HDE::Status HDE::Server::get_status()
{
	return this->status;
}

void HDE::Server::responder()
{
	this->extension = extract_extension(header[1]);
	this->file_path = build_path(header[1]);

	switch (this->status)
	{
		case NEW: case ERROR_PENDING:
			if (header[0] == "GET" && is_allowed_method(header[0]))
				handleGet(target_socket);
			else if (header[0] == "POST" && is_allowed_method(header[0]))
				handlePost(target_socket);
			else if (header[0] == "DELETE" && is_allowed_method(header[0]))
				handleDelete(target_socket);
			else
				send_error_page(target_socket, "405");
			break;
		case SENDING_CHUNK:
			send_chunk();
			break;
		case DONE:
			this->status = NEW;
			break;
		default:
			break;
	}
}

bool HDE::Server::is_allowed_method(string method)
{
	vector<string> allowedMethodVector;

	if (this->in_path.empty())
		allowedMethodVector = config->get_allowed_methods();
	else
	{
		vector<CONF::ServerLocation> locationVector = config->get_locations();
		for (vector<CONF::ServerLocation>::iterator it = locationVector.begin(); it != locationVector.end(); it++)
			if (it->get_path() == this->in_path && it->get_allowed_methods().size() != 0)
				allowedMethodVector = it->get_allowed_methods();
	}
	if (allowedMethodVector.size() == 0)
		allowedMethodVector = config->get_allowed_methods();

	if(std::find(allowedMethodVector.begin(), allowedMethodVector.end(), method) != allowedMethodVector.end())
		return true;
	return false;
}

string HDE::Server::build_path(string url)
{
	string base, finalPath, root = "", index = "";
	vector<CONF::ServerLocation> locationVector = config->get_locations();
	int baseEnd, pathEnd;

	if (url.substr(1).find("?") != string::npos) // from /wow/wow.png?yea=yea, get [/wow/wow.png]?yea=yea
		pathEnd = url.substr(1).find("?");
	else
		pathEnd = url.substr(1).size();

	base = url.substr(0, pathEnd + 1);

	std::sort(locationVector.begin(), locationVector.end(), CONF::ServerLocation::compareByLength);
	for (vector<CONF::ServerLocation>::iterator it = locationVector.begin(); it != locationVector.end(); it++)
	{
		if ((baseEnd = is_defined_path(it->get_path(), base)))
		{
			this->in_path = it->get_path();
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
		finalPath = root + url.substr(baseEnd, pathEnd + 1);

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

int HDE::Server::is_defined_path(string conf_path, string client_path)
{
	if (client_path == "/" && conf_path == "/")
		return 1;
	else if (client_path == "/" && conf_path != "/")
		return 0;

	string base;
	size_t baseEnd, nextSlash;
	baseEnd = 0;
	nextSlash = 1;

	while (baseEnd < client_path.size())
	{
		if (client_path.find("/", nextSlash) != string::npos) // from /wow/wow.png, get [/wow]/wow.png
			baseEnd = client_path.find("/", nextSlash);
		else
			baseEnd = client_path.size();
		base = client_path.substr(0, baseEnd);
		if (conf_path == base)
			return baseEnd;
		nextSlash = baseEnd + 1;
	}
	return 0;
}

void HDE::Server::send_error_page(int socket, string error_code)
{
	string error_text, errorFilePath;
	std::stringstream response, responseTmp;
	bool getFromFile = false;
	std::map<string, string> userDefinedErr = config->get_error_page();

	for(std::map<string, string>::iterator it = userDefinedErr.begin(); it != userDefinedErr.end(); it++)
	{
		if (it->first == error_code)
		{
			getFromFile = true;
			errorFilePath = it->second;
		}
	}

	cout << "[INFO] Sending error file" << endl;
	error_text = get_error_text(error_code);

	response << "HTTP/1.1 " << error_code << " " << error_text << "\r\n";
	response << "Connection: keep-alive\r\n";
	response << "Content-Type: text/html\r\n\r\n";

	if (getFromFile == true)
	{
		string fileData;
		std::ifstream fileO;
		char fileBuffer[BUFFER_SIZE];

		fileO.open(errorFilePath.c_str());
		if (fileO.is_open())
		{
			while (fileO.read(fileBuffer, sizeof(fileBuffer)))
				fileData.append(fileBuffer, sizeof(fileBuffer));
			fileData.append(fileBuffer, fileO.gcount());
			fileO.close();
			response << fileData;
		}
		else
		{
			cout << "[ERROR] Error opening error file" << endl;
			getFromFile = false;
		}
	}
	if (getFromFile == false)
	{
		responseTmp << "<!DOCTYPE html>";
		responseTmp << "<html>";
		responseTmp << "<head>";
		responseTmp << "<title>ERROR " << error_code << "</title>";
		responseTmp << "</head>";
		responseTmp << "<body>";
		responseTmp << "<h1>" << error_code << " " << error_text << "</h1>";
		responseTmp << "</body>";
		responseTmp << "</html>";
		response << responseTmp.str();
	}

	this->status = DONE;
	send_whole(socket, response.str());
	return;
}

string HDE::Server::get_error_text(string error_code)
{
	if (error_code == "400")
		return (string("Bad Request"));
	else if (error_code == "401")
		return (string("Unauthorized"));
	else if (error_code == "403")
		return (string("Forbidden"));
	else if (error_code == "404")
		return (string("Not Found"));
	else if (error_code == "405")
		return (string("Method Not Allowed"));
	else if (error_code == "413")
		return (string("Request Entity Too Large"));
	else if (error_code == "500")
		return (string("Internal Server Error"));
	else if (error_code == "501")
		return (string("Not Implemented"));
	else if (error_code == "505")
		return (string("HTTP Version Not Supported"));
	else
		return (string("Error Not Found"));
}

int	HDE::Server::get_socket()
{
	return this->target_socket;
}

const CONF::ServerConfig *HDE::Server::get_config()
{
	return (config);
}

vector<string> HDE::Server::get_header()
{
	return (header);
}

string HDE::Server::get_headers()
{
	return (headers);
}

string HDE::Server::get_content()
{
	return (content);
}

vector<string>	HDE::Server::chopString(string str, string delimiter)
{
	vector<string> v;
    size_t		start = 0;
    size_t		end = 0;

	while ((start = str.find_first_not_of(delimiter, end)) != string::npos)
	{
		end = str.find_first_of(delimiter, start + 1);
        v.push_back(str.substr(start, end - start));
    }
	return (v);
}
