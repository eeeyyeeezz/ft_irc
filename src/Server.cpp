#include "../inc/Server.hpp"
#include "../inc/User.hpp"
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

Server::Server(int port) : _port(port), _countConnects(0) {
	struct pollfd fds[50];
	// _fds = struct pollfd[50];
}

int		Server::getPort() { return(_port); }

void	Server::setListening(int socket) { _listening = socket; }

int		Server::getListening() { return(_listening); }

int		Server::getCountConnects() { return(_countConnects); }

void	Server::setCountConnects(int i) { _countConnects += i; }

void	Server::createSocket(Server &server){
	server.setListening(socket(AF_INET, SOCK_STREAM, 0));
	if (server.getListening() == -1) { error("Error establishing connection"); } 
}

void	Server::bindSocket(Server &server){
	int flagOne = 1;
	if (setsockopt(server.getListening(), SOL_SOCKET, SO_REUSEADDR, &flagOne, sizeof(int)) < 0){ error("setsockpt error"); }

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(server.getPort());
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	
	if (bind(server.getListening(), (sockaddr *)&hint, sizeof(hint)) == -1) { error("Can't bind"); } 
}

void	Server::listenSocket(Server &server, struct pollfd fds[]){
	if (listen(server.getListening(), SOMAXCONN) == -1) { error("Can't listen"); } 
	fds[0].fd = server.getListening();
	fcntl(fds[0].fd, F_SETFL, O_NONBLOCK);
}

void	Server::mainLoop(Server &server, struct pollfd fds[]){
	int flag = 0;
	while (true){
		int COUNTFD;
		
		if (flag > 0) { std::cout << "Exit\n" ; exit(EXIT_SUCCESS); } // exit_success
		if ((COUNTFD = poll(fds, server.getCountConnects(), -1)) < 0) { error("Poll crash"); } // do_error poll crash

		for (size_t i = 0; i < server.getCountConnects(); i++){
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN) == POLLIN){
				++flag;
				if (i == 0){ // new_conenct
					flag = 0;
					fds[server.getCountConnects()].fd = accept(fds[i].fd, NULL, NULL);
					std::cout << "NEW CONNNECT\n";
					fds[server.getCountConnects()].events = POLLIN;
					fds[server.getCountConnects()].revents = 0;
					server.setCountConnects(1);
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
						server.setCountConnects(-1);
						continue ;
					}
					buff[readed] = 0;
					std::cout << "Message: " << buff;
					User(std::string(buff));
					for (size_t userToWrite = 0; userToWrite < server.getCountConnects(); userToWrite++){
						if (fds[i].fd != fds[userToWrite].fd)
							send(fds[userToWrite].fd, buff, readed + 1, 0);
					}
					fds[i].revents = 0;
				}
			}
		}
	}
}

Server::~Server(){ }