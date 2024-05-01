#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../srcs/srcs.hpp"

const int BUFFER_SIZE = 1024;

namespace CONF
{
	class Config;
	class ServerConfig;
}

using CONF::Config;

namespace HDE
{
	class Server
	{
		private:
			int target_socket;
			const CONF::ServerConfig	*config;

			vector<string>	header;
			string			headers;
			string			content;


		public:
			Server(const CONF::ServerConfig *config, int client_fd);
			~Server();

			// Server.cpp
			int accepter();
			void handler();
			void responder();

			int get_socket();
			string	get_headers();
			string	get_content();
			vector<string> get_header();
			const CONF::ServerConfig *get_config();

			vector<string>	chopString(string str, string delimiter);

			// Get.cpp
			void handleGet(int socket);

			void startLogin(int socket);
			void doLogin(int socket, string uname, string pwd);
			void doRegister(int socket, string uname, string pwd);

			void error(int socket, string type);
			void html(int socket, string new_url);
			void png(int socket, string new_url);
			void ico(int socket, string new_url);
			void cgi(int socket);
			char** query(string url);

			// Post.cpp
			void handlePost(int socket);
			void dataHandle(int socket);
			void parsebody();
			void cutstr(size_t pos, size_t size);

			// Delete.cpp
			void handleDelete(int socket);

	};
}

#endif