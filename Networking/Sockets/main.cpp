#include "ListeningSocket.hpp"

int main()
{
	std::cout << "Starting....." << std::endl;
	std::cout << "Binding Socket....." << std::endl;
	HDE::BindingSocket bs = HDE::BindingSocket(AF_INET, SOCK_STREAM, 0, 0, INADDR_ANY);
	std::cout << "Listening Socket....." << std::endl;
	HDE::ListeningSocket ls = HDE::ListeningSocket(AF_INET, SOCK_STREAM, 0, 0, INADDR_ANY, 10);

}

// === ALLOW PORT 80 ACCESS ===
// sudo ufw allow 80/tcp

// g++ main.cpp SimpleSocket.cpp ListeningSocket.cpp BindingSocket.cpp -o test

// === RUN AS SUDO; FIXED PERMISSION DENIED ERROR (ERRNO 13) ===
// sudo ./test
