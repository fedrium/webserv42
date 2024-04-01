#include "srcs.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		string file = argv[1];
		ParseConfig	servers;
		servers.parseServers(file);
		servers.showInfo();
	}
	else
		cout << "Please input: ./webserv [configuration file]" << endl;
}

// g++ main.cpp ParseConfig.cpp ServerConfig.cpp ServerLocation.cpp 