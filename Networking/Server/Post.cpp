#include "Server.hpp"

void HDE::Server::parsebody()
{
	cout << "in" << std::endl;

	char quote = '\"';
	this->body.clear();
	this->filename = content.substr(content.find("filename") + 1);
	cout << "in2" << std::endl;
	this->filename = this->filename.substr(this->filename.find(quote) + 1, this->filename.find(quote));
	cout << "filename: " << this->filename << " len: " << this->content_length << std::endl;
	temp_body = chopString(content, "\n");
	vector<string>::iterator ptr;
	int i = 0;
	for (ptr = temp_body.begin(); ptr < temp_body.end(); ptr++)
		i++;
	int j = 0;
	for (ptr = temp_body.begin(); ptr < temp_body.end(); ptr++)
	{
		if (j > 2 && j < i - 2)
			this->body.append(*ptr).append("\n");
		j++;
	}

	cout << "body: " << this->body << std::endl;

	std::fstream fs(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
	fs << this->body;
	fs.close();
}

void HDE::Server::handlePost(int socket)
{
	cout << "inhere" << std::endl;
	parsebody();
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
