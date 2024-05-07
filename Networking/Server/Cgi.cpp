#include "Server.hpp"

char*	dynamicDup(string s)
{
	char *str = new char[s.length() + 1];
	strcpy(str, s.c_str());
	return str;
}

void HDE::Server::cutstr(size_t pos, size_t size)
{
	this->content.erase(pos, size);
}

char* char_substr(string &str, size_t start, size_t length) {
	cout << "in" << std::endl;
    char* result = new char[length + 1];
    std::strncpy(result, str.c_str() + start, length);
    result[length] = '\0';

    return result;
}

vector<char *> HDE::Server::queryString(string query)
{
	vector<char *> res;
	int start = 0;
	for (size_t i = 0; i < query.length(); i++)
	{
		if (query[i] == '&')
		{
			res.push_back(char_substr(query, start, (i - start)));
			start = i + 1;
		}
	}
	res.push_back(char_substr(query, start, query.length() - start));
	for (vector<char *>::iterator ptr = res.begin(); ptr < res.end(); ptr++)
		cout << "ENV: " << *ptr << std::endl;
	return res;
}

void HDE::Server::cgi(int socket)
{
	std::cout << "URL: " << this->header[1] << std::endl;
	int status;
	// char **env;
	vector<char *> env;
	char buf[BUFFER_SIZE];

	// if (header[1].find("?") != std::string::npos)
	// 	env = queryString(header[1].substr(header[1].find("?") + 1, std::string::npos));

	if (header[1].find("?") != std::string::npos)
	{
		env = queryString(header[1].substr(header[1].find("?") + 1, std::string::npos));
	}
	env.push_back(NULL);
	string parse_filepath;
	parse_filepath = this->file_path;

	string output;
	char **args = new char*[3];
	args[0] = dynamicDup("/usr/bin/python3");
	args[1] = dynamicDup(parse_filepath);
	args[2] = NULL;

	int readfd[2];
	int writefd[2];

	if (pipe(readfd) == -1 || pipe(writefd) == -1)
		return;
	int pid = fork();
	if (pid == 0)
	{
		dup2(readfd[1], STDOUT_FILENO);
		dup2(writefd[0], STDIN_FILENO);
		close(readfd[0]);
		close(readfd[1]);
		close(writefd[0]);
		close(writefd[1]);

		execve("/usr/bin/python3", args, env.data());
		std::cerr << "execve failed" << std::endl;
		exit(1);
	}
	else
	{
		while (!this->content.empty())
		{
			if (this->content.length() > BUFFER_SIZE)
			{
				write(writefd[1], this->content.substr(0, BUFFER_SIZE).c_str(), BUFFER_SIZE);
				this->cutstr(0, BUFFER_SIZE);
			}
			else
			{
				write(writefd[1], this->content.c_str(), this->content.size());
				this->cutstr(0, this->content.size());
			}
		}
		close(writefd[1]);
		close(writefd[0]);
		waitpid(pid, &status, 0);
		close(readfd[1]);
		// int end;
		while (1)
		{
			int bytes_read = read(readfd[0], buf, BUFFER_SIZE);
			if (bytes_read <= 0)
				break;
			buf[bytes_read] = '\0';
			output.append(buf);
		}
		output.append("\0");
		output.append("\r\n\r\n");
		close(readfd[0]);
		delete[] args[0];
		delete[] args[1];
		delete[] args;
		if (header[1].find("?") != string::npos){
			for (size_t i = 0; i < env.size(); i++)
				delete[] env[i];
		}
		cout << "CGI: " << output << std::endl;
		send_whole(socket, output);
	}
	return;
}
