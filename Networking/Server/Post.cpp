#include "Server.hpp"

void HDE::Server::handlePost(int socket)
{
	// parsebody();
	cgi(socket);
	// error(socket, "404");
}

// replace this whole thing and upload.py
// void HDE::Server::cgi(int socket)
// {
// 	vector<string> args = chopString(content, "\r\n");
// 	vector<string> tmp;
// 	string command, filename;
// 	for (int i = 0; i < args.size(); i++)
// 		cout << args[i] << endl;
// 	tmp = chopString(args[1], ";");
// 	filename = chopString(tmp[tmp.size() - 1], "=")[1];
// 	command = "python3 public/cgi/upload.py " + filename;
// 	for (int i = 3; i < args.size() - 1; i++)
// 		command.append(" ").append("\"").append(args[i]).append("\"");
// 	cout << command << endl;
// 	system(command.c_str());
// 	png(socket, "/i_forgor.png");
// }
