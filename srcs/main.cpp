#include "srcs.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		string file = argv[1];
		CONF::Config	config;
		config.parseServers(file);
		// config.showInfo();
		cout << "Run with sudo if permission denied since port 80 is privileged" << endl;
		HDE::Server		servers(&config);
	}
	else
		cout << "Please input: ./webserv [configuration file]" << endl;
}

// g++ main.cpp ParseConfig.cpp ServerConfig.cpp ServerLocation.cpp 