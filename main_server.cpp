#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "inc/Server.hpp"
#include "inc/User.hpp"

void	InitialSetup(struct pollfd fds[], int count)
{
	for (size_t i = 0; i < count; i++)
		fds[i].fd = -1;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
}

void	newConnect(int &listening){
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	
	int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
	if (clientSocket == -1) { error("Client connecting error"); }
	
	close(listening);
	
	memset(host, 0, NI_MAXHOST); 
	memset(svc, 0, NI_MAXSERV);
	
	int result = getnameinfo((sockaddr * )&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	if (result){
		std::cout << host << " connected on " << svc << std::endl;
 	} else { 
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); 
		std::cout << host << " connected on " << htons(client.sin_port) << std::endl;
	}
}

int		main(int argc, char **argv){
	if (argc != 2) { error("Args Error. Pls type: <port> <password>"); } 
	
	// Первичная настройка
	Server	server(atoi(argv[1]));
	struct pollfd fds[10]; // количество FD'шников
	InitialSetup(fds, 10);
	
	server.setCountConnects(1);

	server.createSocket(server);
	
	server.bindSocket(server);
	
	server.listenSocket(server, fds);

	server.mainLoop(server, fds);

	// close(clientSocket);
}