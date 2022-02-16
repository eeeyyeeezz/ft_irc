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

	int		commandStart(Server &server, struct pollfd fds[]);
	void	checkCommand(Server &server, struct pollfd fds[]);

	// COMMANDS
	void	doQuitCommand(Server &server, struct pollfd fds[]);
	void	doNickCommand(Server &server);
	void	doPrivmsgCommand(Server &server);
	//void	doChannelPrivmsg(Channel &tmpChannel, int _fd, string message);
	void	doNoticeCommand(Server &server);
	
	// CHANNEL COMMANDS
	void	doJoinCommand(Server &server);
	void	doPartCommand(Server &server);
	void	doKickCommand(Server &server);

	// BOT
	void	doHelpCommand();
	void	doInfoCommand(Command &user);
	void	doShowusersCommand(Server &server);
	void	doShowtimeCommand();
	void	doRandnumberCommand();
	
		
	// OTHER
	bool	checkUserInChannel(Channel &channel);
};


