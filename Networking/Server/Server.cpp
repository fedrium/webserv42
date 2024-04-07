#include "Server.hpp"

HDE::Server::Server(CONF::Config *servers) : SimpleServer (AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10), config(config)
{
	launch();
}

void HDE::Server::accepter()
{
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	new_socket = accept(get_socket()->get_sock(), 
		(struct sockaddr *)&address, (socklen_t *)&addrlen);
	read(new_socket, buffer, sizeof(buffer));
	
	vector<string> headerVector = chopString(buffer, " ");
	header.clear();
	for (int i = 0; i != 3; i++)
		header.push_back(headerVector[i]);
	for (int i = 0; i != 3; i++)
		cout << header[i] << endl;

}

void HDE::Server::handler()
{
	std::cout << buffer << std::endl;
}

void HDE::Server::responder()
{
	char *text;
	if (header[1] == "/cum")
		text = (char *)"I will fking piss white cement it will gush out of my pipe";
	else if (header[1] == "/wtf")
		text = (char *)"Nah bruh tf is this holy bs wallahi im finished";
	else if (header[1] == "/ded")
		text = (char *)"*dies*";
	else
		text = (char *)"Always bet on hakari";
	write(new_socket, text, strlen(text));
	close(new_socket);
}

void HDE::Server::launch()
{
	while (true)
	{
		std::cout << "===== WAITING =====" << std::endl;
		accepter();
		handler();
		responder();
		std::cout << "===== DONE =====" << std::endl;
	}
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
