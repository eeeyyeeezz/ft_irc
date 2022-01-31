#include "../inc/Server.hpp"
#include "../inc/User.hpp"

#define BUFFER_SIZE 4096

Server::Server(int port, string password) : _port(port), _password(password), _countConnects(0) {
	struct pollfd fds[50];
	// _fds = struct pollfd[50];
}

int		Server::getPort() { return(_port); }

void	Server::setListening(int socket) { _listening = socket; }

int		Server::getListening() { return(_listening); }

int		Server::getCountConnects() { return(_countConnects); }

string	Server::getPassword() { return(this->_password); }

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

void	Server::writeToServerAndAllUsers(string buff, int readed, struct pollfd fds[], int i){
	std::cout << "Message: " << buff;
	for (size_t userToWrite = 0; userToWrite < this->getCountConnects(); userToWrite++){
		if (fds[i].fd != fds[userToWrite].fd)
			send(fds[userToWrite].fd, buff.c_str(), readed + 1, 0);
	}
}

void	Server::setNewConnection(User &user, int &flag, struct pollfd fds[], size_t &i){
	flag = 0;
	fds[this->getCountConnects()].fd = accept(fds[i].fd, NULL, NULL);
	// setFD to User
	user.setFd(fds[this->getCountConnects()].fd);
	std::cout << "NEW CONNNECT\n";
	send(user.getFd(), "With first log in type PASS and password\n", 41 + 1, 0);
	fds[this->getCountConnects()].events = POLLIN;
	fds[this->getCountConnects()].revents = 0;
	this->setCountConnects(1);
}

void	Server::continueConnection(User &user, int &flag, struct pollfd fds[], size_t &i){
	char buff[BUFFER_SIZE];
	flag = 0;
	
	int readed = read(fds[i].fd, buff, BUFFER_SIZE);
	fds[i].revents = 0;
	if (!readed){
		std::cout << fds[i].fd << "  disconnected\n";
		fds[i].fd = -1;
		this->setCountConnects(-1);
	}
	buff[readed] = 0;
	if (!user.parsCommand(std::string(buff)))
		this->writeToServerAndAllUsers(std::string(buff), readed, fds, i);
	fds[i].revents = 0;
}

void	Server::mainLoop(Server &server, User &user, struct pollfd fds[]){
	int flag = 0;
	while (true){
		int COUNTFD;
		
		if (flag > 0) { std::cout << "Exit\n" ; exit(EXIT_SUCCESS); } // exit_success
		if ((COUNTFD = poll(fds, server.getCountConnects(), -1)) < 0) { error("Poll crash"); } // do_error poll crash

		for (size_t i = 0; i < server.getCountConnects(); i++){
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN) == POLLIN){
				++flag;
				if (i == 0) 
					server.setNewConnection(user, flag, fds, i);
				else
					server.continueConnection(user, flag, fds, i);
			}
		}
	}
}

Server::~Server(){ }