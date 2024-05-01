SOCKET_DIR	= Networking/Sockets
SOCKET_FILE	= SimpleSocket.cpp BindingSocket.cpp ConnectingSocket.cpp ListeningSocket.cpp

SERVER_DIR	= Networking/Server
SERVER_FILE	= Server.cpp ServerListener.cpp Get.cpp Post.cpp Delete.cpp

SRCS_DIR = srcs/
SRCS_FILE = Config.cpp ServerConfig.cpp ServerLocation.cpp main.cpp Webserv.cpp

O_DIR = obj

CC			= g++
RM			= rm -rf
CFLAGS		=

NAME		= webserv

SOCKET	= $(addprefix $(SOCKET_DIR)/, $(notdir $(SOCKET_FILE)))
SERVER	= $(addprefix $(SERVER_DIR)/, $(notdir $(SERVER_FILE)))
SRCS	= $(addprefix $(SRCS_DIR)/, $(notdir $(SRCS_FILE)))

# SRCS_O	:= $(addprefix $(O_DIR)/,$(notdir $(SRCS_FILE:.cpp=.o)))
# SERVER_O	:= $(addprefix $(O_DIR)/,$(notdir $(SERVER_FILE:.cpp=.o)))
# SOCKET_O	:= $(addprefix $(O_DIR)/,$(notdir $(SOCKET_FILE:.cpp=.o)))

# vpath %.cpp $(SOCKET_DIR) $(SRCS_DIR) $(SERVER_DIR)

all:	${NAME}

${NAME}: ${SOCKET} ${SERVER} ${SRCS}
	${CC} ${CFLAGS} ${SOCKET} ${SERVER} ${SRCS} -o ${NAME}

# ${NAME}: ${SOCKET_O} ${SERVER_O} ${SRCS_O}
# 	g++ $(CFLAGS) $^ -o $@

# $(O_DIR) :
# 	@mkdir -p $@
# 	@mkdir -p Users

# $(O_DIR)/%.o: %.cpp | $(O_DIR)
# 	g++ $(CFLAGS) -c $< -o $@;

clean:
	${RM} ${NAME}

fclean: clean
	${RM} ${NAME}
#	${RM} ${O_DIR}

re: fclean all

.PHONY: all