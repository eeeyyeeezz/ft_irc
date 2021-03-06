#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "inc/Server.hpp"
#include "inc/User.hpp"

void	InitialSetup(Server &server, struct pollfd fds[], int count)
{
	server.setCountConnects(1);
	for (size_t i = 0; i < (size_t)count; i++)
		fds[i].fd = -1;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
}

int		main(int argc, char **argv){
	if (argc != 3) { error("Args Error. Please type: <port> <password>"); } 
		
	// Первичная настройка
	Server	server(atoi(argv[1]), std::string(argv[2]));
	struct pollfd fds[10]; // количество FD'шников
	InitialSetup(server, fds, 10);

	server.createSocket(server);
	
	server.bindSocket(server);
	
	server.listenSocket(server, fds);

	server.mainLoop(server, fds);
}