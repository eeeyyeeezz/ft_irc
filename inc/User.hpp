#ifndef USER_HPP
# define USER_HPP

#include "GlobalLibraries.hpp"
using std::string;

#define BUFFER_SIZE 4096

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

	User(User const & );
	User &operator = (User const & );
	void checkUserPassword(string message);

	public:
	User();
	User(int fd);
	User(string password);
	// User(string message);
	User(string username, string hostname, string servername, string realname);
	~User();
	
	void	setFd(int fd);
	int		getFd();
	int		parsCommand(string message); // return 1 - user passed command ; return 0 - simple message

};

#endif