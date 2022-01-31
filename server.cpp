#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include "inc/Server.hpp"

#define BUFFER_SIZE 4096

void	InitialSetup(struct pollfd fds[], int count)
{
	for (size_t i = 0; i < count; i++)
		fds[i].fd = -1;
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
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int countconnects = 0;
	countconnects++;

	server.createSocket(server);
	
	server.bindSocket(server);
	
	server.listenSocket(server, fds);
	
	// Принятие и вывод сообщений
	int flag = 0;
	while (true){
		int COUNTFD;
		
		if (flag > 0) { std::cout << "Exit\n" ; exit(EXIT_SUCCESS); } // exit_success
		if ((COUNTFD = poll(fds, countconnects, -1)) < 0) { error("Poll crash"); } // do_error poll crash

		for (size_t i = 0; i < countconnects; i++){
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN) == POLLIN){
				++flag;
				if (i == 0){ // new_conenct
					flag = 0;
					fds[countconnects].fd = accept(fds[i].fd, NULL, NULL);
					std::cout << "NEW CONNNECT\n";
					// newConnect(listening);					
					fds[countconnects].events = POLLIN;
					fds[countconnects].revents = 0;
					++countconnects;
				}
				else{
					char buff[BUFFER_SIZE];
					flag = 0;
					
					int readed;
					readed = read(fds[i].fd, buff, BUFFER_SIZE);
					fds[i].revents = 0;
					if (!readed){
						std::cout << fds[i].fd << "  disconnected\n";
						fds[i].fd = -1;
						--countconnects;
						continue ;
					}
					buff[readed] = 0;
					std::cout << "Message: " << buff;
					for (size_t userToWrite = 0; userToWrite < countconnects; userToWrite++)
						send(fds[userToWrite].fd, buff, readed + 1, 0);
					fds[i].revents = 0;
				}
			}
		}
	}

	// close(clientSocket);
}