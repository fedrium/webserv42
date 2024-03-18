# SOCKET		= Networking/Sockets/SimpleSocket.cpp \
# 				Networking/Sockets/ConnectingSocket.cpp \
# 				Networking/Sockets/BindingSocket.cpp \
# 				Networking/Sockets/ListeningSocket.cpp 

# SERVER		= Networking/Server/SimpleServer.cpp \
# 				Networking/Server/Server.cpp \
# 				Networking/Server/main.cpp

SOCKET_DIR	= Networking/Sockets
SOCKET_FILE	= SimpleSocket.cpp BindingSocket.cpp ConnectingSocket.cpp ListeningSocket.cpp

SERVER_DIR	= Networking/Server
SERVER_FILE	= Server.cpp SimpleServer.cpp main.cpp

CC			= g++
RM			= rm -rf
CFLAGS		= -std=c++98

NAME		= webserv

SOCKET		= $(addprefix $(SOCKET_DIR)/, $(notdir $(SOCKET_FILE)))
SERVER		= $(addprefix $(SERVER_DIR)/, $(notdir $(SERVER_FILE)))

# SOCKET_O	= $(SOCKET:.cpp=.o)
# SERVER_O	= $(SERVER:.cpp=.o)


all:	${NAME}

${NAME}: ${SOCKET} ${SERVER}
	${CC} ${CFLAGS} ${SOCKET} ${SERVER} -o ${NAME}

clean:
	${RM} ${NAME}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all