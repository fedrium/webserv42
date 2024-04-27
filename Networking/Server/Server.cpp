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
	int byteread = read(new_socket, buffer, sizeof(buffer));
	
	cout << "Byteread: " << byteread << std::endl;

	vector<string> tmpHeaderVector = chopString(buffer, " \n");
	header.clear();
	for (int i = 0; i != 3; i++)
		header.push_back(tmpHeaderVector[i]);
	
	vector<string> tmpContentVector = chopString(buffer, "\n");
	misc.clear();
	for (int i = 1; i < tmpContentVector.size(); i++)
	{
		if (!tmpContentVector[i].find("Content-Length: "))
			this->content_length = tmpContentVector[i].substr(15, string::npos);
		if (!tmpContentVector[i].find("----"))
			this->bound = tmpContentVector[i];
		misc.push_back(tmpContentVector[i]);
	}

	content = "";
	int i = -1;
	while (++i < misc.size())
		if (misc[i].find("--") == 0)
			break;
	if (i < misc.size())
		for (int j = i; j < misc.size(); j++)
			content += misc[j].append("\n");
	std::cout << "content: " << content << std::endl;
}

void HDE::Server::handler()
{
	cout << "HHeader: ";
	for (int i = 0; i != header.size(); i++)
		cout << header[i] << " ";
	cout << endl;
}

void HDE::Server::responder()
{
	if (header[0] == "GET")
		handleGet(new_socket);
	else if (header[0] == "POST")
	{
		// vector<string>::iterator ptr;
		// for (ptr = misc.begin(); ptr < misc.end(); ptr++)
		// 	cout << *ptr << std::endl;
		handlePost(new_socket);
	}
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
