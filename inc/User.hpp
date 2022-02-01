#ifndef USER_HPP
# define USER_HPP
#include "GlobalLibraries.hpp"
#include "Server.hpp"

class Server;

class User {

	private:
	string	_username;
	string	_hostname;		// не проверяется 
	string	_servername;		// не проверяется 
	string	_realname;
	string	_password;
	int		_passwordPassed;
	int		_nickNamePassed;
	int		_userPassed;
	string	_nickname;
	int		_sockfd;
	int		_port;

	User &operator = (User const & );
	void checkUserPassword(Server &server, string message, int fd, int i);

	public:
	User();
	User(int fd);
	User(string password);
	User(string username, string hostname, string servername, string realname);
	~User();
	
	// SETTERS
	void	setFd(int fd);
	void	setPasswordPassed();
	void	setNickName(string nickName);
	
	// GETTERS
	int		getFd();
	bool	getAllPrepArguments();

	// PARSING
	int		preparationCommands(Server &server, string message, int i);
	int		parsCommand(Server &server, string message, int i); // return 1 - user passed command ; return 0 - simple message
};

#endif