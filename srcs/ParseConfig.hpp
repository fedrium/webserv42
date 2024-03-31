#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP

#include "srcs.hpp"
#include "ServerConfig.hpp"

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;

class ServerConfig;

class ParseConfig
{
	private:
		std::vector<ServerConfig>	servers;

	public:
		ParseConfig();
		~ParseConfig();

		void				showInfo();
		void				parseServers(string file);
		vector<string>		chopString(string str);
};

#endif