#include "ParseConfig.hpp"

ParseConfig::ParseConfig()
{

}

ParseConfig::~ParseConfig()
{

}

void	ParseConfig::parseServers(string file)
{
	std::ifstream	filestream(file.c_str());
	string			line;
	vector<string>	info;
	int				i = -1;

	while (std::getline(filestream, line))
	{
		info = chopString(line);
		if (info.empty())
			continue;
		if (info[0] == "server")
		{
			this->servers.push_back(ServerConfig());
			i++;
		}
		else if (this->servers.size())
			servers[i].parseInfo(info);
	}
}

vector<string>	ParseConfig::chopString(string str)
{
	vector<string> v;
    int		start = 0;
    int		end = 0;
	string	delimiter = " \t;";

	while ((start = str.find_first_not_of(delimiter, end)) != string::npos)
	{
		if (str[start] == '#')
			break;
		end = str.find_first_of(delimiter, start + 1);
        v.push_back(str.substr(start, end - start));
    }
	return (v);
}

void	ParseConfig::showInfo()
{
	for (int i = 0; i < this->servers.size(); i++)
		this->servers[i].printInfo();
	cout << "Total number of servers: " << this->servers.size() << endl;
}