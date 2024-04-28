#include "ServerConfig.hpp"

CONF::ServerConfig::ServerConfig()
{
	this->root = "";
	this->index = "";
	this->server_name = "localhost";
	this->client_max = "";
}

CONF::ServerConfig::~ServerConfig()
{

}

void CONF::ServerConfig::parseInfo(vector<string> info)
{
	static unsigned int in_location_block = 0;

	if (info[0] == "location")
		in_location_block += 1;

	if (info[0] == "}" && in_location_block % 2 == 1)
		in_location_block += 1;

	if (in_location_block % 2 == 0)
		set_server_attr(info);
	else
		set_locations(info);
}

void	CONF::ServerConfig::set_server_attr(vector<string> info)
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
}

void	CONF::ServerConfig::printInfo()
{
	cout << "Ports: ";
	for (std::vector<string>::const_iterator i = this->ports.begin(); i != this->ports.end(); ++i)
    	cout << *i << ' ';
	cout << endl;

	cout << "Root: " << get_root() << endl;
	cout << "Index: " << get_index() << endl;
	cout << "Server name: " << get_server_name() << endl;
	cout << "Client max body size: " << get_client_max() << endl;

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

	cout << "Locations: " << endl;
	for (int i = 0; i < this->locations.size(); i++)
    	this->locations[i].printInfoLocation();
	
	cout << endl << "=============================================================================" << endl << endl;

}

void	CONF::ServerConfig::set_ports(vector<string> info)
{
	for (int i = 1; i < info.size(); i++)  
        this->ports.push_back(info[i]);
}

void	CONF::ServerConfig::set_root(vector<string> info)
{
	this->root = info[1];
}

void	CONF::ServerConfig::set_index(vector<string> info)
{
	this->index = info[1];
}

void	CONF::ServerConfig::set_server_name(vector<string> info)
{
	this->server_name = info[1];
}

void	CONF::ServerConfig::set_client_max(vector<string> info)
{
	this->client_max = info[1];
}

void	CONF::ServerConfig::set_allowed_methods(vector<string> info)
{
	for (int i = 1; i < info.size(); i++)
        this->allowed_methods.push_back(info[i]);
}

void	CONF::ServerConfig::set_error_page(vector<string> info)
{
	this->error_page[info[1]] = info[2];
}

void	CONF::ServerConfig::set_cgi(vector<string> info)
{
	this->cgi[info[1]] = info[2];
}

void	CONF::ServerConfig::set_locations(vector<string> info)
{
	static int i = -1;

	if (!this->locations.size())
		i = -1;

	if (info[0] == "location")
	{
		this->locations.push_back(ServerLocation());
		i++;
		locations[i].set_path(info);
	}
	else if (this->locations.size())
		locations[i].parseInfoLocation(info);
}

string	CONF::ServerConfig::get_root()
{
	return (this->root);
}

string	CONF::ServerConfig::get_index()
{
	return (this->index);
}

string	CONF::ServerConfig::get_server_name()
{
	return (this->server_name);
}

string	CONF::ServerConfig::get_client_max()
{
	return (this->client_max);
}

const std::vector<string>	CONF::ServerConfig::get_ports() const
{
	return (this->ports);
}

std::vector<string>	CONF::ServerConfig::get_allowed_methods()
{
	return (this->allowed_methods);
}

std::map<string, string>	CONF::ServerConfig::get_error_page()
{
	return (this->error_page);
}

std::map<string, string>	CONF::ServerConfig::get_cgi()
{
	return (this->cgi);
}

std::vector<ServerLocation>	CONF::ServerConfig::get_locations()
{
	return (this->locations);
}
