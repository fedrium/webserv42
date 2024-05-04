#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../srcs/srcs.hpp"

const int BUFFER_SIZE = 1024;
const int CHUNK_SIZE = 16000;

namespace CONF
{
	class Config;
	class ServerConfig;
}

using CONF::Config;

namespace HDE
{
	enum Status {
		NEW,
		SENDING_CHUNK,
		ERROR,
		DONE
	};

	class Server
	{
		private:
			int target_socket;
			const CONF::ServerConfig	*config;
			Status status;

			vector<string>	header;
			string			headers;
			string			content;
			string			extension;
			string			file_path;
			std::ifstream	fstream_for_chunk;
			int				chunk_times;

			string			content_length;
			string			bound;
			string			bound_one;
			vector<string>	temp_body;
			string			body;


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
			Status get_status();

			vector<string>	chopString(string str, string delimiter);

			// Get.cpp
			void send_chunk();
			void send_whole(int socket, string data);
			string get_content_type(string extension);
			string extract_extension(string url);
			void handleGet(int socket);
			int is_redirect(string url);

			void startLogin(int socket);
			void doLogin(int socket, string uname, string pwd);
			void doRegister(int socket, string uname, string pwd);

			void error(int socket, string type);
			void html(int socket, string new_url);
			void png(int socket, string new_url);
			void ico(int socket, string new_url);
			void cgi(int socket);
			vector<char *> queryString(string query);

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