#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
	
int		main(){
	// Создать сокет
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1) { } // do_error
	
	// Связать сокет с айпи / портом
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); // 6667 ??
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) { } // do_error can't bind
	
	// Обозначить какой сокет слушать
	if (listen(listening, SOMAXCONN) == -1) { } // do_error can't listen
	
	// Принять звонок
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	
	int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
	if (clientSocket == -1) { } // do_error problem with client connecting
	
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
	
	// Принятие и вывод сообщений
	char buf[4096];
	while (true){
		memset(buf, 0, 4096);
		int bytesRecieved = recv(clientSocket, buf, 4096, 0);
		if (bytesRecieved == -1) { } // do_error connection problem
		if (bytesRecieved == 0) { } // client dissconected
		
		std::cout << "Recieved: " << std::string(buf, 0, bytesRecieved) << std::endl;
		send(clientSocket, buf, bytesRecieved + 1, 0);
		
	}
	
	close(clientSocket);
}