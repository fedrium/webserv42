#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	this->ports.push_back("80");
	this->root = "";
	this->index = "";
	this->server_name = "localhost";
	this->client_max = "1m";
	this->allowed_methods.push_back("GET");

}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::parseInfo(vector<string> info)
{
	if (info[0] == "listen")
		set_ports(info);
	else if (info[0] == "root")
		set_root(info);
	else if (info[0] == "index")
		set_index(info);
	else if (info[0] == "server_name")
		set_server_name(info);
	else if (info[0] == "client_max_body_size")
		set_client_max(info);
	else if (info[0] == "allowed_methods")
		set_allowed_methods(info);
	else if (info[0] == "error_page")
		set_error_page(info);
	else if (info[0] == "cgi_script")
		set_cgi(info);
	else
		set_locations(info);	
}

void	ServerConfig::printInfo()
{
	cout << "Ports: ";
	for (std::vector<string>::const_iterator i = this->ports.begin(); i != this->ports.end(); ++i)
    	cout << *i << ' ';
	cout << endl;

	cout << "Root: " << this->root << endl;
	cout << "Index: " << this->index << endl;
	cout << "Server name: " << this->server_name << endl;
	cout << "Client max body size: " << this->client_max << endl;

	cout << "Allowed methods: ";
	for (std::vector<string>::const_iterator i = this->allowed_methods.begin(); i != this->allowed_methods.end(); ++i)
    	cout << *i << ' ';
	cout << endl;

	cout << "Error pages (Shown below): " << endl;
	for (std::map<string, string>::const_iterator i = this->error_page.begin(); i != this->error_page.end(); ++i)
    	cout << "    " << i->first << " " << i->second << endl;

	cout << "CGI stuff (Shown below): " << endl;
	for (std::map<string, string>::const_iterator i = this->cgi.begin(); i != this->cgi.end(); ++i)
    	cout << "    " << i->first << " " << i->second << endl;

	
	cout << endl << "=============================================================================" << endl << endl;

}

void	ServerConfig::set_ports(vector<string> info)
{
	this->ports.clear();
	for (int i = 1; i < info.size(); i++)  
        this->ports.push_back(info[i]);
}

void	ServerConfig::set_root(vector<string> info)
{
	this->root = info[1];
}

void	ServerConfig::set_index(vector<string> info)
{
	this->index = info[1];
}

void	ServerConfig::set_server_name(vector<string> info)
{
	this->server_name = info[1];
}

void	ServerConfig::set_client_max(vector<string> info)
{
	this->client_max = info[1];
}

void	ServerConfig::set_allowed_methods(vector<string> info)
{
	this->allowed_methods.clear();
	for (int i = 1; i < info.size(); i++)
        this->allowed_methods.push_back(info[i]);
}

void	ServerConfig::set_error_page(vector<string> info)
{
	this->error_page[info[1]] = info[2];
}

void	ServerConfig::set_cgi(vector<string> info)
{
	this->cgi[info[1]] = info[2];
}

void	ServerConfig::set_locations(vector<string> info)
{
	
}

