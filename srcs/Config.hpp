#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "srcs.hpp"
#include "ServerConfig.hpp"

class ServerConfig;

namespace CONF
{
	class Config
	{
		private:
			vector<ServerConfig>	servers;

		public:
			Config();
			~Config();
			Config(const Config &config);

			void				showInfo();
			void				parseServers(string file);
			vector<string>		chopString(string str);
			const vector<ServerConfig>	getServers() const;
	};
}

#endif