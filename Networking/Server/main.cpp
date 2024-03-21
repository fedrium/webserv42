#include "Server.hpp"

int main()
{
	HDE::Server s;
}

// g++ Server/main.cpp Server/Server.cpp Server/SimpleServer.cpp Sockets/SimpleSocket.cpp Sockets/BindingSocket.cpp Sockets/ListeningSocket.cpp -o test
// sudo ./test

// sudo netstat -tulpn
// checks for all processes connected to a port and their pid and local address

// change port in Server.cpp to 80 or others if not working