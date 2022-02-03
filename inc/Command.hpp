#pragma once
#include "GlobalLibraries.hpp"
#include "User.hpp"

class User;

class Command{
	private:
	int				_fd;
	string			_nickname;
	string			_command;
	vector<User>	_users;
	vector<string>	_arguments;

	public:
	Command();
	Command(string message, int fd, string nickname, vector<User> users);
	~Command();

	int		commandStart();
	void	checkCommand();
	void	doQuitCommand();
	void	doNickCommand();

};


