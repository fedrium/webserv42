#include "ServerConfig.hpp"

CONF::ServerConfig::ServerConfig()
{
	this->root = "";
	this->index = "";
	this->server_name = "localhost";
	this->client_max = "";
	this->count = 0;
}

CONF::ServerConfig::~ServerConfig()
{

}

void CONF::ServerConfig::parseInfo(vector<string> info)
{
	static unsigned int in_location_block = 0;

	// cout << "this is info:: " << info[0] << endl;
	if (info[0] == "location"){
		if (in_location_block % 2 == 1){
			count++;
		}
		else
			in_location_block += 1;
	}

	// if (info[0] == "location" && in_location_block % 2 == 0){
	// 	count++;
	// 	in_location_block -= 1;
	// }
	if (info[0] == "}" && count % 2 == 1){
		// cout << "in nest" << endl;
		in_location_block += 1;
		vector<string> tmp;
		tmp.push_back("");
		tmp.push_back("");
		set_locations(tmp);
	}

	if (info[0] == "}" && in_location_block % 2 == 1){
		// cout << "in normal" << endl;
		in_location_block += 1;
	}

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

	cout << "----------info::  " << info[0] << "   :: path ::  "<< info[1] << endl;
	if (info[0] == "location")
	{
		this->locations.push_back(ServerLocation());
		i++;
		locations[i].set_path(info);
	}

	else if (this->locations.size()){

		// cout << "count:: " << count << endl << "count % 2 :: " << count % 2 << endl;
		while (count % 2 == 1){
			// cout << "incre:: " << count << endl;
			if (info[0] == "root" || info[0] == "autoindex" || info[0] == "index"
				|| info[0] == "return" || info[0] == "alias" || info[0] == "client_max_body_size"
				|| info[0] == "allowed_methods")
				break ;
			if (locations[i].get_autoindex() == ""){
				info[0].assign("autoindex");
				info[1].assign(locations[i - 1].get_autoindex());
				locations[i].parseInfoLocation(info);
			}
			if (locations[i].get_index() == ""){
				info[0].assign("index");
				info[1].assign(locations[i - 1].get_index());
				locations[i].parseInfoLocation(info);
			}
			if (locations[i].get_return_url() == ""){
				info[0].assign("return");
				info[1].assign(locations[i - 1].get_return_url());
				locations[i].parseInfoLocation(info);
			}
			if (locations[i].get_alias() == ""){
				info[0].assign("alias");
				info[1].assign(locations[i - 1].get_alias());
				locations[i].parseInfoLocation(info);
			}
			if (locations[i].get_client_max_body_size() == ""){
				info[0].assign("client_max_body_size");
				info[1].assign(locations[i - 1].get_client_max_body_size());
				locations[i].parseInfoLocation(info);
			}
			else
				break ;
			count++;
			return ;
		}
		// cout << "I ran" << endl;
		locations[i].parseInfoLocation(info);
	}
}

// void	CONF::ServerConfig::set_nest_locations(vector<string> info)
// {
// 	static int i = -1;

// 	if (!this->locations.size())
// 		i = -1;

// 	cout << "----------info::  " << info[0] << "   :: path ::  "<< info[1] << endl;
// 	if (info[0] == "location")
// 	{
// 		this->locations.push_back(ServerLocation());
// 		vector<string> nest;
// 		nest.push_back("a");
// 		string tmp;

// 		tmp.append(locations[count - 1].get_path());

// 		i++;
// 		count++;
// 		tmp.append(info[1]);
// 		nest.push_back(tmp);
// 		locations[i].set_path(nest);
// 	}
// 	else if (this->locations.size())
// 		locations[i].parseInfoLocation(info);
// }

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
