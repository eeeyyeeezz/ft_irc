#pragma once
#include "GlobalLibraries.hpp"
#include "User.hpp"
#include "Server.hpp"

class User;
class Server;

class Command{
	private:
	int				_fd;
	int				_i;
	string			_nickname;
	string			_command;
	vector<string>	_arguments;

	public:
	vector<User>	_users;
	Command();
	Command(string message, int fd, string nickname, vector<User> &users);
	~Command();

	// GETTERS
	vector<User>	getVectorOfUsers();

	int		commandStart(Server &server);
	void	checkCommand(Server &server);

	// COMMANDS
	void	doQuitCommand();
	void	doNickCommand(Server &server);
	void	doPrivmsgCommand();
	void	doHelpCommand();
};


