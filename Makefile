NAME = ircserv

SRCS = main_server.cpp \
		src/Utils.cpp src/Server.cpp \
		src/User.cpp src/Command.cpp src/Channel.cpp \
		src/Bot.cpp

CLANG = clang++

FLAGS = -Wall -Wextra -Werror

RM = rm -rf

all: 
	${CLANG} ${SRCS} -o ${NAME} && ./${NAME} 6667 12345

clean:
	rm -rf server

re: clean all 
 
PHOHY: all re clean fclean 