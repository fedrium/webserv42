#include "Server.hpp"

void HDE::Server::handlePost(int socket)
{
	string userInput, userUsername, userPassword;
	userInput = this->content.back();
	vector<string> tmpInfo1 = chopString(userInput, "&");
	userUsername = chopString(tmpInfo1[0], "=")[1];
	userPassword = chopString(tmpInfo1[1], "=")[1];

	cout << userInput << endl;
	cout << userPassword << endl;
	// le test
	if (userPassword == "forgor" || userPassword == "diodio")
	{
		if (userPassword == "diodio")
			header[1] = "/dio.png";
		else if (userPassword == "forgor")
			header[1] = "/i_forgor.png";
		png(socket, ".png");
	}
	else
	{
		header[1] = "cum.html";
		html(socket, ".html");
	}
}