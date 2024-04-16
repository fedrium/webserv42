#include "Server.hpp"

void HDE::Server::handlePost(int socket)
{
	// cout << this->content.back() << endl;
	    for (int i = 0; i < content.size(); i++) {
        cout << content[i] << endl;
			}
	if (header[1] == "/login" || header[1] == "/register")
	{
		string userInput, userUsername, userPassword;

		userInput = this->content.back();
		vector<string> tmpInfo1 = chopString(userInput, "&");
		userUsername = chopString(tmpInfo1[0], "=")[1];
		userPassword = chopString(tmpInfo1[1], "=")[1];

		cout << userInput << endl;
		cout << userPassword << endl;

		if (header[1] == "/login")
			doLogin(socket, userUsername, userPassword);
		else if (header[1] == "/register")
			doRegister(socket, userUsername, userPassword);
	}
}

void HDE::Server::doLogin(int socket, string uname, string pwd)
{
	std::ifstream	csvFile("./database/login.csv");
	string line, formatToCompare;
	bool validLogin = 0;

	formatToCompare = uname + "," + pwd;
	while (std::getline(csvFile, line))
	{
		if (formatToCompare == line)
			validLogin = 1;
	}
	if (validLogin)
	{
		header[1] = "/i_forgor.png";
		png(socket, ".png");
	}
	else
	{
		header[1] = "/login_fail.html";
		html(socket, ".html");
	}
}

void HDE::Server::doRegister(int socket, string uname, string pwd)
{
	std::ofstream csvFile;
    csvFile.open("./database/login.csv", std::ios::out | std::ios::app);
    csvFile << uname << "," << pwd << endl;
	csvFile.close();
	header[1] = "/reg_success.html";
	html(socket, ".html");
}

void HDE::Server::doUnregister(int socket, string uname, string pwd)
{
	// std::ofstream csvFile;
    // csvFile.open("./database/login.csv", std::ios::out | std::ios::app);
    // csvFile << uname << "," << pwd << endl;
	// csvFile.close();
	// header[1] = "/reg_success.html";
	// html(socket, ".html");
}