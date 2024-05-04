#include "Server.hpp"

void HDE::Server::handlePost(int socket)
{
	if (this->status == ERROR_PENDING || this->status == ERROR)
	{
		cout << "[ERROR] An error has been detected. Aborting request..." << endl;
		cout << "heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
		send_error_page(socket, "413");
		this->status = DONE;
		return;
	}
	// parsebody();
	cgi(socket);
	this->status = DONE;
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
