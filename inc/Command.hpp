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

	vector<User>	_users;
	vector<string>	_arguments;

	public:
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
	void	doPrivmsgCommand(int flag);
	void	doNoticeCommand();
	void	doHelpCommand();	
	
	// CHANNEL COMMANDS
	void	doJoinCommand(Server &server);
	void	doPartCommand();

};


