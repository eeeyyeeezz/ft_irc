#include "../inc/User.hpp"

User::User() { };

User::User(int fd) : _sockfd(fd) { }

User::User(string password) : _password(password), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) { }

User::User(string username, string hostname, string servername, string realname) 
: _username(username), _hostname(hostname), _servername(servername), _realname(realname) {

}

int			User::parsCommand(string message){
	string firstCommands[3] = {"PASS", "NICK", "USER"};

	if (!this->_passwordPassed && message == "PASS\n"){
		this->checkUserPassword();
		return (1);
	} else if (!this->_passwordPassed && message != "PASS") {
		send(this->getFd(), "You need to write PASS command and password\n", 45, 0);
		return (1);	
	}

	// if (!this->_passwordPassed && message != "PASS"){
	// 	send(this->getFd(), "You need to pass PASS command and password\n", 44, 0);
	// } else if (!this->_passwordPassed && message == "PASS"){
	// 	send(this->getFd(), "Type pass: ", 12 + 1, 0);
	// }
	return (0);
}

void			User::checkUserPassword(){


	send(this->getFd(), "Passed password is ", 20 + 1, 0);
}

void			User::setFd(int fd) { _sockfd = fd; }

int				User::getFd() { return(this->_sockfd); }

User::~User() { };