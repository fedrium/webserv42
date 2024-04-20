#include "Server.hpp"

void HDE::Server::handlePost(int socket)
{
	if (header[1] == "/login" || header[1] == "/register")
	{
		string userInput, userUsername, userPassword;

		userInput = this->content.back();
		vector<string> tmpInfo1 = chopString(userInput, "&");

		cout << userInput << endl;

		if (tmpInfo1.size() != 3)
			return error(socket, "400");
		if (chopString(tmpInfo1[0], "=").size() != 2 || chopString(tmpInfo1[1], "=").size() != 2)
			return error(socket, "400");

		userUsername = chopString(tmpInfo1[0], "=")[1];
		userPassword = chopString(tmpInfo1[1], "=")[1];

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
		png(socket, "/i_forgor.png");
	else
		html(socket, "/login_fail.html");
}

void HDE::Server::doRegister(int socket, string uname, string pwd)
{
	std::ofstream csvFile;
	std::ifstream cmpCsvFile;
	string line, unameRegged;
	cmpCsvFile.open("./database/login.csv");
	while (std::getline(cmpCsvFile, line))
	{
		unameRegged = chopString(line, ",")[0];
		if (uname == unameRegged)
		{
			html(socket, "/acc_exists.html");
			cmpCsvFile.close();
			return;
		}
	}
	cmpCsvFile.close();
    csvFile.open("./database/login.csv", std::ios::out | std::ios::app);
    csvFile << uname << "," << pwd << endl;
	csvFile.close();
	html(socket, "/reg_success.html");
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