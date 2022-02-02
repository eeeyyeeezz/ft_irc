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
	// string	_hostname;		// не проверяется 
	// string	_servername;		// не проверяется 
	// string	_realname;
	int		_passwordPassed;
	int		_nickNamePassed;
	int		_userPassed;
	int		_sockfd;

	User &operator = (User const & );
	void checkUserPassword(Server &server, string message, int i);

	public:
	User();
	User(int fd);
	~User();
	
	// SETTERS
	void	setUsername(string username);
	void	setNickname(string nickname);

	void	setFd(int fd);
	void	setPasswordPassed();
	void	setNicknamePassed();
	void	setUserPassed();
	
	// GETTERS
	int		getFd();
	int		getPasswordPassed();
	int		getNickNamePassed();
	int		getUserPassed();

	string	getUsername();
	string	getNickname();
	bool	getAllPrepArguments();

	// PARSING
	int		preparationCommands(Server &server, string message, int i);
	int		parsNickCommand(Server &server, string message, int i);
	int		parsUserCommand(Server &server, string message, int i);
	int		parsCommand(Server &server, string message, int i); // return 1 - user passed command ; return 0 - simple message
};

#endif