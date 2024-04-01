#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "srcs.hpp"
#include "ServerConfig.hpp"

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;

class ServerConfig;

namespace CONF
{
	class Config
	{
		private:
			std::vector<ServerConfig>	servers;

		public:
			Config();
			~Config();
			Config(const Config &config);

			void				showInfo();
			void				parseServers(string file);
			vector<string>		chopString(string str);
	};
}

#endif