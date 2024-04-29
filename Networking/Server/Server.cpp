#include "Server.hpp"

HDE::Server::Server(const CONF::ServerConfig *config, int client_fd)
{
	this->target_socket = client_fd;
	this->config = config;
}

HDE::Server::~Server()
{
}

int HDE::Server::accepter()
{
	string request;
	char buffer[BUFFER_SIZE];
	int bytesRead;
	while ((bytesRead = read(this->target_socket, buffer, sizeof(buffer))) > 0)
	{
		request.append(buffer, bytesRead);
		int endOfHeader = request.find("\r\n\r\n");
		if (endOfHeader != string::npos)
		{
			headers = request.substr(0, endOfHeader + 4); // is entire header
			content = request.substr(endOfHeader + 4); // is everything after entire header
			header.clear(); // is general header
			for (int i = 0; i < 3; i++)
				header.push_back(chopString(headers, " \r\n")[i]);

			int contentLengthPos = headers.find("Content-Length: ");
			if (contentLengthPos != std::string::npos) // if Content-Length header is present
			{
				int beforeLenValuePos = contentLengthPos + string("Content-Length: ").length(); // e.g. Content-Length : (here)123
				int afterLenValuePos = headers.find("\r\n", beforeLenValuePos); // e.g. Content-Length : 123(here)

				int contentLength = atoi((headers.substr(beforeLenValuePos, afterLenValuePos - beforeLenValuePos)).c_str()); // get content length as int from string
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

void HDE::Server::handler()
{
	cout << "[INFO] General Header Received: ----------------------" << endl;
	for (int i = 0; i < header.size(); i++)
		cout << header[i] << " ";
	cout << endl;
	cout << "[INFO] Header Received: ------------------------------" << endl;
	cout << headers << endl;
	cout << "[INFO] Content Received: -----------------------------" << endl;
	// cout << content << endl;
	cout << endl;
}

void HDE::Server::responder()
{
	if (header[0] == "GET")
		handleGet(target_socket);
	else if (header[0] == "POST")
		handlePost(target_socket);
	else if (header[0] == "DELETE")
		handleDelete(target_socket);
	else
		error(target_socket, "405");
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
    int		start = 0;
    int		end = 0;

	while ((start = str.find_first_not_of(delimiter, end)) != string::npos)
	{
		end = str.find_first_of(delimiter, start + 1);
        v.push_back(str.substr(start, end - start));
    }
	return (v);
}
