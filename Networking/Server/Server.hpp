#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../srcs/srcs.hpp"

const int BUFFER_SIZE = 1024;
const int CHUNK_SIZE = 16000;

namespace CONF
{
	// class Config;
	class ServerConfig;
	// class ServerLocation;
}

namespace HDE
{
	enum Status {
		NEW,
		SENDING_CHUNK,
		ERROR,
		ERROR_PENDING,
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
			string			redirect_url;
			bool			autoindex;
			string			error_code;
			string			in_path;

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
			
			int get_client_max_server();
			bool is_allowed_method(string method);
			string build_path(string url);
			int is_defined_path(string conf_path, string client_path);
			void send_error_page(int socket, string error_code);
			string get_error_text(string error_code);

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
			int handle_redirect(int socket, string url);

			// Cgi.cpp
			vector<char *> queryString(string query);
			void cgi(int socket);

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