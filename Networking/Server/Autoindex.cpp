#include "Server.hpp"
#include "iomanip"
#include "dirent.h"

//encoding the path name so characters that are not allowed in the url bar can be displayed
//such as %20 replacing a space or %3A replacing colon (:)
string HDE::Server::encode_url(const string &value)
{
	cout << "encode_url" << endl;

	std::stringstream path;
	path << std::hex;

	for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i){
		string::value_type c = (*i);

		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~'){
			path << c;
			continue ;
		}
		path << std::uppercase;
		path << '%' << std::setw(2) << int((unsigned char) c);
		path << std::nouppercase;
	}
	return (path.str());
}

void HDE::Server::create_html(int socket, vector<ServerLocation> sl)
{
	cout << "create_html" << endl;

	std::stringstream	html_str;
	vector<string>		name, date;
	vector<int>			size;

	struct stat			buf;
	int					err;

	struct dirent		*de;
	struct tm			*timeinfo;
	DIR					*directory;

	char				buffer[80];

	html_str << "<!DOCTYPE html>" << endl;
	html_str << "<html>" << endl;
	html_str << "<style>" << endl;
	html_str << "table, th, td {\nborder:1.5px solid black;\n}" << endl;
	html_str << "</style>" << endl;
	html_str << "<body>" << endl;
	html_str << "<h1> Index of " << this->header[1] << "</h1>" << endl;
	html_str << "<table style=\"width:100%\">" << endl;
	html_str << "<tr><th>Name</th><th>Date</th><th>Size</th></tr>" << endl;

	cout << "sc path= " << sl[0].get_path() << endl;
	cout << "sc path= " << sl[1].get_path() << endl;
	cout << "sc path= " << sl[2].get_path() << endl;
	cout << "sc path= " << sl[3].get_path() << endl;
	cout << "sc path= " << sl[4].get_path() << endl;
	cout << "sc path= " << sl[5].get_path() << endl;

	//doesnt work without the period
	cout << "." << this->header[1] << endl;
	int	iter = 0;
	for (; iter != sl.capacity(); ++iter){
		if (sl[iter].get_path().find(this->header[1]) != string::npos)
			break ;
	}
	cout << "after finding = " << sl[iter].get_path() << endl;
	cout << "." << sl[iter].get_path() << endl;

	cout << "root = " << sl[iter].get_path() << endl;

	directory = opendir(sl[iter].get_root().c_str());
	string	path, domain_path, encoded_path;

	for (de = readdir(directory); de != NULL; de = readdir(directory))
	{
		path = sl[iter].get_root() + de->d_name;
		cout << "path = " << path << endl;
		domain_path = sl[iter].get_path() + "/" + de->d_name;
		cout << "domain_path = " << domain_path << endl;

		err = stat(path.c_str(), &buf);

		if (string(de->d_name) == ".")
			continue ;
		else if (string(de->d_name) == ".."){
			if (sl[iter].get_path().find('/', 1) != string::npos)
				domain_path = sl[iter].get_path().substr(0, sl[iter].get_path().find('/', 1));
			else
				domain_path = "/";
		}
		if (err < 0)
			cout << "⌠ stat() failed ⌡" << endl;
		else{
			html_str << "<tr>" << endl;
			encoded_path = encode_url(domain_path);
			html_str << "<td><a href=\"" << encoded_path << "\">" << de->d_name << "</a></td>" << endl;
			//cplusplus.com strftime
			timeinfo = localtime(&(buf.st_mtime));
			strftime(buffer, 80, "%e-%B-%Y", timeinfo);
			html_str << "<td>" << buffer << "</td>" << endl;
			if (S_ISREG(buf.st_mode))
				html_str << "<td>" << buf.st_size << "</td>" << endl;
			else
				html_str << "<td> - </td>" << endl;
			html_str << "</tr>" << endl;
		}
	}
	html_str << "</table>" << endl;
	html_str << "</body>" << endl;
	html_str << "</html>" << endl;
	cout << "0000000000000000000000000000" << endl;
	cout << html_str.str() << endl;
	cout << "0000000000000000000000000000" << endl;
	send_whole(this->target_socket, html_str.str().c_str());
}
