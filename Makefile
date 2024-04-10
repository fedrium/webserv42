SOCKET_DIR	= Networking/Sockets
SOCKET_FILE	= SimpleSocket.cpp BindingSocket.cpp ConnectingSocket.cpp ListeningSocket.cpp

SERVER_DIR	= Networking/Server
SERVER_FILE	= Server.cpp SimpleServer.cpp Get.hpp

SRCS_DIR = srcs/
SRCS_FILE = Config.cpp ServerConfig.cpp ServerLocation.cpp main.cpp

O_DIR = obj

CC			= g++
RM			= rm -rf
CFLAGS		= 

NAME		= webserv

# SOCKET		= $(addprefix $(SOCKET_DIR)/, $(notdir $(SOCKET_FILE)))
# SERVER		= $(addprefix $(SERVER_DIR)/, $(notdir $(SERVER_FILE)))
# SRCS		= $(addprefix $(SRCS_DIR)/, $(notdir $(SRCS_FILE)))

SRCS_O := $(addprefix $(O_DIR)/,$(notdir $(SRCS_FILE:.cpp=.o)))
SERVER_O := $(addprefix $(O_DIR)/,$(notdir $(SERVER_FILE:.cpp=.o)))
SOCKET_O := $(addprefix $(O_DIR)/,$(notdir $(SOCKET_FILE:.cpp=.o)))

vpath %.cpp $(SOCKET_DIR) $(SRCS_DIR) $(SERVER_DIR)

all:	${NAME}

${NAME}: ${SOCKET} ${SERVER} ${SRCS}
	${CC} ${CFLAGS} ${SOCKET} ${SERVER} ${SRCS} -o ${NAME}

clean:
	${RM} ${NAME}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all