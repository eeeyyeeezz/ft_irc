NAME = ircserv

SRCS = main_server.cpp \
		src/Utils.cpp src/Server.cpp \
		src/User.cpp src/Command.cpp src/Channel.cpp \
		src/Bot.cpp

CLANG = c++

FLAGS = -Wall -Wextra -Werror

RM = rm -rf

all: 
	${CLANG} ${FLAGS} -g ${SRCS} -o ${NAME}

run: 
	${CLANG} ${FLAGS} -g ${SRCS} -o ${NAME} && ./${NAME} 6667 5555

clean:
	rm -rf ${NAME}

re: clean all 
 
PHOHY: all re clean fclean 