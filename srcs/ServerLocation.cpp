#include "ServerLocation.hpp"

ServerLocation::ServerLocation()
{
	this->path = "";
	this->root = "";
	this->autoindex = "";
	this->index = "";
	this->return_url = "";
	this->alias = "";
	this->client_max_body_size = "";
}

ServerLocation::~ServerLocation()
{

}

void	ServerLocation::parseInfoLocation(vector<string> info)
{
	if (info[0] == "root")
		set_root(info);
	else if (info[0] == "autoindex")
		set_autoindex(info);
	else if (info[0] == "index")
		set_index(info);
	else if (info[0] == "allowed_methods")
		set_allowed_methods(info);
	else if (info[0] == "return")
		set_return_url(info);
	else if (info[0] == "alias")
		set_alias(info);
	else if (info[0] == "client_max_body_size")
		set_client_max_body_size(info);
}

void	ServerLocation::printInfoLocation()
{
	cout << "   Path: " << get_path() << endl;
	if (!get_root().empty())
		cout << "       Root: " << get_root() << endl;
	if (!get_autoindex().empty())
		cout << "       Autoindex: " << get_autoindex() << endl;
	if (!get_index().empty())
		cout << "       Index: " << get_index() << endl;
	
	if (!get_allowed_methods().empty())
	{
		cout << "       Allowed methods: ";
		for (std::vector<string>::const_iterator i = this->allowed_methods.begin(); i != this->allowed_methods.end(); ++i)
			cout << *i << ' ';
		cout << endl;
	}

	if (!get_return_url().empty())
		cout << "       Return: " << get_return_url() << endl;
	if (!get_alias().empty())
		cout << "       Alias: " << get_alias() << endl;
	if (!get_client_max_body_size().empty())
		cout << "       Client max body size: " << get_client_max_body_size() << endl;
}

void	ServerLocation::set_path(std::vector<string> info)
{
	this->path = info[1];
}

void	ServerLocation::set_root(std::vector<string> info)
{
	this->root = info[1];
}

void	ServerLocation::set_allowed_methods(std::vector<string> info)
{
	for (int i = 1; i < info.size(); i++)
		this->allowed_methods.push_back(info[i]);
}

void	ServerLocation::set_autoindex(std::vector<string> info)
{
	this->autoindex = info[1];
}

void	ServerLocation::set_index(std::vector<string> info)
{
	this->index = info[1];
}

void	ServerLocation::set_return_url(std::vector<string> info)
{
	this->return_url = info[1];
}

void	ServerLocation::set_alias(std::vector<string> info)
{
	this->alias = info[1];
}

void	ServerLocation::set_client_max_body_size(std::vector<string> info)
{
	this->alias = info[1];
}

string				ServerLocation::get_path()
{
	return (this->path);
}

string				ServerLocation::get_root()
{
	return (this->root);
}

vector<string>		ServerLocation::get_allowed_methods()
{
	return (this->allowed_methods);
}

string				ServerLocation::get_autoindex()
{
	return (this->autoindex);
}

string				ServerLocation::get_index()
{
	return (this->index);
}

string				ServerLocation::get_return_url()
{
	return (this->return_url);
}

string				ServerLocation::get_alias()
{
	return (this->alias);
}

string				ServerLocation::get_client_max_body_size()
{
	return (this->client_max_body_size);
}
