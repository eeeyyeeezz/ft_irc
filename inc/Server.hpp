#pragma once
#include "User.hpp"
#include "GlobalLibraries.hpp"

using std::string;

class Server{
	private:
	int				_port;
	int				_listening;
	int				_countConnects;
	string			_password;
	struct pollfd	_fds[50];

	public:
	Server(int port, string password);
	~Server();

	void	createSocket(Server &server);
	void	bindSocket(Server &server);
	void	listenSocket(Server &server, struct pollfd fds[]);
	int		getPort();
	int		getListening();
	int		getCountConnects();
	string	getPassword();
	void	mainLoop(Server &server, User &user, struct pollfd fds[]);
	void	setCountConnects(int i);
	void	setListening(int socket);
};


void	error(string error);