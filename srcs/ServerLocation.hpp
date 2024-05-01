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
			void	printInfoLocation() const;

			void 	set_path(std::vector<string> info);
			void 	set_root(std::vector<string> info);
			void 	set_allowed_methods(std::vector<string> info);
			void 	set_autoindex(std::vector<string> info);
			void 	set_index(std::vector<string> info);
			void 	set_return_url(std::vector<string> info);
			void	set_alias(std::vector<string> info);
			void 	set_client_max_body_size(std::vector<string> info);

			const string				get_path() const;
			const string				get_root() const;
			const vector<string>		get_allowed_methods() const;
			const string				get_autoindex() const;
			const string				get_index() const;
			const string				get_return_url() const;
			const string				get_alias() const;
			const string				get_client_max_body_size() const;
	};
}

#endif