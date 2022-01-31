#pragma once
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <poll.h>

using std::string;

class Server{
	private:
	int				_port;
	int				_listening;
	struct pollfd	_fds[50];

	public:
	Server(int port);
	~Server();

	void	createSocket(Server &server);
	void	bindSocket(Server &server);
	void	listenSocket(Server &server, struct pollfd fds[]);
	int		getPort();
	int		getListening();
	void	setListening(int socket);
};


void	error(string error);