#ifndef USER_HPP
# define USER_HPP
#include "GlobalLibraries.hpp"
#include "Server.hpp"
#include "Command.hpp"

class Server;

class User {

	private:
	string	_nickname;
	string	_username;
	int		_atChannelFd; // atChannelID
	int		_sockfd;
	
	// VALIDATION FLAGS
	int		_passwordPassed;
	int		_nickNamePassed;
	int		_userPassed;

	void checkUserPassword(Server &server, string message, int i);

	public:
	User();
	User(int fd);
	~User();
	
	// SETTERS
	void	setUsername(string username);
	void	setNickname(string nickname);
	void	setAtChannelFd(int fd);
	void	setFd(int fd);

	void	setPasswordPassed();
	void	setNicknamePassed();
	void	setUserPassed();
	
	// GETTERS
	int		getFd();
	int		getPasswordPassed();
	int		getNickNamePassed();
	int		getUserPassed();
	int		getAtChannelFd();

	string	getUsername();
	string	getNickname();
	bool	getAllPrepArguments();

	// PARSING
	int		preparationCommands(Server &server, string message, int i);
	int		parsNickCommand(Server &server, string message, int i);
	int		parsUserCommand(Server &server, string message, int i);
	int		parsCommand(Server &server, string message, int i, struct pollfd fds[]); // return 1 - user passed command ; return 0 - simple message

	void sendError(string err);
};

#endif