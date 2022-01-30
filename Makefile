NAME = server

SRCS = server.cpp

CLANG = clang++

FLAGS = -Wall -Wextra -Werror

RM = rm -rf

all: 
	${CLANG} ${SRCS} -o ${NAME} && ./${NAME} 6667

clean:
	rm -rf server

re: clean all 
 
PHOHY: all re clean fclean 