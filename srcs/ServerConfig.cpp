#include "ServerConfig.hpp"

CONF::ServerConfig::ServerConfig()
{
	this->root = "";
	this->index = "";
	this->server_name = "localhost";
	this->client_max = "";
	this->count = 0;
}

CONF::ServerConfig::ServerConfig(const CONF::ServerConfig &obj)
{
	this->ports = obj.ports;
	this->root = obj.root;
	this->index = obj.index;
	this->server_name = obj.server_name;
	this->client_max = obj.client_max;
	this->allowed_methods = obj.allowed_methods;
	this->error_page = obj.error_page;
	this->cgi = obj.cgi;
	this->locations = obj.locations;
	this->count = obj.count;
}

CONF::ServerConfig::~ServerConfig()
{

}

void CONF::ServerConfig::parseInfo(vector<string> info)
{
	static unsigned int in_location_block = 0;

	if (info[0] == "location"){
		if (in_location_block % 2 == 1){
			++count;
			set_locations(info, true);
		}
		else
			in_location_block += 1;
	}

	if (info[0] == "}" && count % 2 == 1){
		in_location_block += 1;
		vector<string> tmp;
		tmp.push_back("");
		tmp.push_back("");
		set_locations(tmp, false);
	}

	if (info[0] == "}" && in_location_block % 2 == 1){
		in_location_block += 1;
	}

	if (in_location_block % 2 == 0)
		set_server_attr(info);

	else
		set_locations(info, false);
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

void	CONF::ServerConfig::printInfo() const
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
	for (size_t i = 0; i < this->locations.size(); i++)
    	this->locations[i].printInfoLocation();

	cout << endl << "=============================================================================" << endl << endl;

}

void	CONF::ServerConfig::set_ports(vector<string> info)
{
	for (size_t i = 1; i < info.size(); i++)
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
	for (long unsigned int i = 1; i < info.size(); i++)
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

void	CONF::ServerConfig::set_locations(vector<string> info, bool nested)
{
	static int i = -1;

	if (nested == true){
		this->Parent = i;
		return ;
	}

	if (!this->locations.size())
		i = -1;

	if (info[0] == "location")
	{
		this->locations.push_back(ServerLocation());
		i++;
		locations[i].set_path(info);
	}

	else if (this->locations.size())
	{
		while (count % 2 == 1){
			if (info[0] == "root" || info[0] == "autoindex" || info[0] == "index"
				|| info[0] == "return" || info[0] == "alias" || info[0] == "client_max_body_size"
				|| info[0] == "allowed_methods")
				break ;
			if (locations[Parent].get_autoindex() != ""){
				info[0].assign("autoindex");
				info[1].assign(locations[Parent].get_autoindex());
				locations[i].parseInfoLocation(info);
			}
			if (locations[Parent].get_index() != ""){
				info[0].assign("index");
				info[1].assign(locations[Parent].get_index());
				locations[i].parseInfoLocation(info);
			}
			if (locations[Parent].get_return_url() != ""){
				info[0].assign("return");
				info[1].assign(locations[Parent].get_return_url());
				locations[i].parseInfoLocation(info);
			}
			if (locations[Parent].get_alias() != ""){
				info[0].assign("alias");
				info[1].assign(locations[Parent].get_alias());
				locations[i].parseInfoLocation(info);
			}
			if (locations[Parent].get_client_max_body_size() != ""){
				info[0].assign("client_max_body_size");
				info[1].assign(locations[Parent].get_client_max_body_size());
				locations[i].parseInfoLocation(info);
			}
			else
				break ;
			++count;
			return ;
		}
		locations[i].parseInfoLocation(info);
	}
	return ;
}

const string	CONF::ServerConfig::get_root() const
{
	return (this->root);
}

const string	CONF::ServerConfig::get_index() const
{
	return (this->index);
}

const string	CONF::ServerConfig::get_server_name() const
{
	return (this->server_name);
}

const string	CONF::ServerConfig::get_client_max() const
{
	return (this->client_max);
}

const std::vector<string>	CONF::ServerConfig::get_ports() const
{
	return (this->ports);
}

const std::vector<string>	CONF::ServerConfig::get_allowed_methods() const
{
	return (this->allowed_methods);
}

const std::map<string, string>	CONF::ServerConfig::get_error_page() const
{
	return (this->error_page);
}

const std::map<string, string>	CONF::ServerConfig::get_cgi() const
{
	return (this->cgi);
}

const std::vector<CONF::ServerLocation>	CONF::ServerConfig::get_locations() const
{
	return (this->locations);
}
