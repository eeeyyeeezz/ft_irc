#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>

#define BUFFER_SIZE 4096

void	InitialSetup(struct pollfd fds[], int count)
{
	for (size_t i = 0; i < count; i++)
		fds[i].fd = -1;
}

int		main(int argc, char **argv){
	if (argc != 2) { std::cout << "Args Error\n"; exit(EXIT_FAILURE); } // do_error argc error
	
	// Первичная настройка
	int port = atoi(argv[1]);
	struct pollfd fds[10]; // количество FD'шников
	InitialSetup(fds, 10);
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int countconnects = 0;
	countconnects++;

	// Создать сокет
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1) { std::cout << "Error establishing connectionr\n"; exit(EXIT_FAILURE); } // do_error error establishing connection
	
	// Связать сокет с айпи / портом
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port); // 6667 ??
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) { std::cout << "Can't bind\n"; exit(EXIT_FAILURE); } // do_error can't bind
	
	// Обозначить какой сокет слушать
	if (listen(listening, SOMAXCONN) == -1) { } // do_error can't listen
	fds[0].fd = listening;

	// Принять звонок
	// sockaddr_in client;
	// socklen_t clientSize = sizeof(client);
	// char host[NI_MAXHOST];
	// char svc[NI_MAXSERV];
	
	// int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
	// if (clientSocket == -1) { std::cout << "Client connecting error\n"; exit(EXIT_FAILURE); } // do_error problem with client connecting
	
	// close(listening);
	
	// memset(host, 0, NI_MAXHOST); 
	// memset(svc, 0, NI_MAXSERV);
	
	// int result = getnameinfo((sockaddr * )&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	// if (result){
	// 	std::cout << host << " connected on " << svc << std::endl;
 	// } else { 
	// 	inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); 
	// 	std::cout << host << " connected on " << htons(client.sin_port) << std::endl;
	// }
	
	// Принятие и вывод сообщений
	int flag = 0;
	while (true){
		int COUNTFD;
		
		if (flag > 0) { std::cout << "Exit\n" ; exit(EXIT_SUCCESS); } // exit_success
		if ((COUNTFD = poll(fds, countconnects, -1)) < 0) { exit(EXIT_FAILURE); } // do_error poll crash

		for (size_t i = 0; i < countconnects; i++){
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN) == POLLIN){
				++flag;
				if (!i){ // new_conenct
					flag = 0;
					fds[countconnects].fd = accept(fds[i].fd, NULL, NULL);
					std::cout << "NEW CONNNECT\n";
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
						std::cout << fds[i].fd << "  end\n";
						fds[i].fd = -1;
						--countconnects;
						continue ;
					}
					buff[readed] = 0;
					std::cout << "Message: " << buff;
					fds[i].revents = 0;
				}
			}
		}
	}

	// close(clientSocket);
}