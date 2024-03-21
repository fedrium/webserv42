#include "Server.hpp"

int main()
{
	HDE::Server s;
}

// g++ Server/main.cpp Server/Server.cpp Server/SimpleServer.cpp Sockets/SimpleSocket.cpp Sockets/BindingSocket.cpp Sockets/ListeningSocket.cpp -o test
// sudo ./test

// Now needs to run sudo; prolly need to write a code that finds a non-priveleged free port (> 1024), rn port 0 means find a free port