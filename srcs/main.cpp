#include "Webserv.hpp"
#include "signal.h"

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	if (ac == 2)
	{
		string file = av[1];
		try
		{
			CONF::Config	configuration;
			configuration.parseServers(file);
			// configuration.showInfo();
			HDE::Webserv		server(&configuration, &configuration);
			server.run_servers();
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << endl;
		}
	}
	else
		cout << "[ERROR] Please input: ./webserv [configuration file]" << endl;
}
