#pragma once
#include "GlobalLibraries.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class User;
class Server;
class Channel;

class Command{
	private:
	int				_fd;
	int				_i;
	string			_message;
	string			_username;
	string			_nickname;
	string			_command;

	vector<User>	_users;
	vector<string>	_arguments;

	public:
	Command();
	Command(string message, int fd, string nickname, string username, vector<User> &users);
	~Command();

	// GETTERS
	vector<User>	getVectorOfUsers();

	int		commandStart(Server &server);
	void	checkCommand(Server &server);

	// COMMANDS
	void	doQuitCommand();
	void	doNickCommand(Server &server);
	void	doPrivmsgCommand(Server &server);
	void	doNoticeCommand(Server &server);
	
	// CHANNEL COMMANDS
	void	doJoinCommand(Server &server);
	void	doPartCommand();

	// BOT
	void	doHelpCommand();	
	void	doShowtimeCommand();
	void	doRandnumberCommand();
    bool    checkUserInChannel(Channel &channel);
};


