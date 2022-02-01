#pragma once
#include "User.hpp"
#include "GlobalLibraries.hpp"

class User;

class Server{
	private:
	int				_port;
	int				_listening;
	int				_countConnects;
	vector<int>		_acceptedUsers;
	string			_password;
	struct pollfd	_fds[50];

	public:
	vector<User>	_users;
	Server(int port, string password);
	~Server();

	void	createSocket(Server &server);
	void	bindSocket(Server &server);
	void	listenSocket(Server &server, struct pollfd fds[]);
	int		getPort();
	int		getListening();
	int		getCountConnects();
	string	getPassword();
	void	mainLoop(Server &server, struct pollfd fds[]);
	void	setCountConnects(int i);
	void	writeToServerAndAllUsers(string buff, int readed, struct pollfd fds[], int i);
	void	setNewConnection(int &flag, struct pollfd fds[], size_t &i);
	void	continueConnection(int &flag, struct pollfd fds[], size_t &i);
	void	setListening(int socket);
	void	acceptedUsersPushBack(int value);
};


void	error(string error);