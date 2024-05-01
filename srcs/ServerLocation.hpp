#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include "srcs.hpp"

namespace CONF
{
	class ServerLocation
	{
		private:
			string					path;
			string					root;
			string					autoindex;
			string					index;
			std::vector<string>		allowed_methods;
			string					return_url;
			string					alias;
			string					client_max_body_size;

		public:
			ServerLocation();
			~ServerLocation();

			void	parseInfoLocation(vector<string> info);
			void	printInfoLocation();

			void 	set_path(std::vector<string> info);
			void 	set_root(std::vector<string> info);
			void 	set_allowed_methods(std::vector<string> info);
			void 	set_autoindex(std::vector<string> info);
			void 	set_index(std::vector<string> info);
			void 	set_return_url(std::vector<string> info);
			void	set_alias(std::vector<string> info);
			void 	set_client_max_body_size(std::vector<string> info);

			const string				get_path() const;
			string				get_root();
			vector<string>		get_allowed_methods();
			string				get_autoindex();
			string				get_index();
			const string				get_return_url() const;
			string				get_alias();
			string				get_client_max_body_size();
	};
}

#endif