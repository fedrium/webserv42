SOCKET_DIR	= Networking/Sockets
SOCKET_FILE	= SimpleSocket.cpp BindingSocket.cpp ConnectingSocket.cpp ListeningSocket.cpp

SERVER_DIR	= Networking/Server
SERVER_FILE	= Server.cpp SimpleServer.cpp

SRCS_DIR = srcs/
SRCS_FILE = ParseConfig.cpp ServerConfig.cpp ServerLocation.cpp main.cpp

CC			= g++
RM			= rm -rf
CFLAGS		= 

NAME		= webserv

SOCKET		= $(addprefix $(SOCKET_DIR)/, $(notdir $(SOCKET_FILE)))
SERVER		= $(addprefix $(SERVER_DIR)/, $(notdir $(SERVER_FILE)))
SRCS		= $(addprefix $(SRCS_DIR)/, $(notdir $(SRCS_FILE)))

# SOCKET_O	= $(SOCKET:.cpp=.o)
# SERVER_O	= $(SERVER:.cpp=.o)

all:	${NAME}

${NAME}: ${SOCKET} ${SERVER} ${SRCS}
	${CC} ${CFLAGS} ${SOCKET} ${SERVER} ${SRCS} -o ${NAME}

clean:
	${RM} ${NAME}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all