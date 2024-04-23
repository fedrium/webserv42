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
	
	vector<string> tmpHeaderVector = chopString(buffer, " \n");
	header.clear();
	for (int i = 0; i != 3; i++)
		header.push_back(tmpHeaderVector[i]);
	
	vector<string> tmpContentVector = chopString(buffer, "\n");
	content.clear();
	for (int i = 1; i < tmpContentVector.size(); i++)
		content.push_back(tmpContentVector[i]);
}

void HDE::Server::handler()
{
	cout << "HHeader: ";
	for (int i = 0; i != header.size(); i++)
		cout << header[i] << " ";
	cout << endl;
	// for (int i = 0; i < content.size(); i++) {
	// cout << content[i] << endl;
	// 	}
}

void HDE::Server::responder()
{
	if (header[0] == "GET")
		handleGet(new_socket);
	else if (header[0] == "POST")
		handlePost(new_socket);
	else if (header[0] == "DELETE")
		handleDelete(new_socket);
	else
		error(new_socket, "405");
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
