#include "../inc/User.hpp"

User::User() { };

User::User(int fd) : _sockfd(fd) { }

User::User(string password) : _password(password), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) { }

User::User(string username, string hostname, string servername, string realname) 
: _username(username), _hostname(hostname), _servername(servername), _realname(realname) {

}

int			User::parsCommand(string message){
	string firstWord = message.substr(0, message.find(" "));

	if (this->_passwordPassed == 0 && firstWord == "PASS"){
		this->checkUserPassword(message);
		return (1);
	} else if (this->_passwordPassed == 0 && firstWord != "PASS") {
		std::cout << this->_passwordPassed << std::endl;
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

void			User::checkUserPassword(string message){
	size_t i = message.find(" ");
	while (message[i] && message[i] == ' ')
		i++;
	if (!message[i]){
		send(this->getFd(), "No arguments at PASS\n", 22, 0);
		close(this->getFd());
	}
	
	string parametr = message.substr(i, message.length());
	send(this->getFd(), "Params is ", 11, 0);
	send(this->getFd(), parametr.c_str(), parametr.length(), 0);
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());

	if (parametr == this->_password){
		send(this->getFd(), "Password correct!\n", 19, 0);
		this->_passwordPassed = 1;
	} else {
		send(this->getFd(), "Password wrong!\n", 17, 0);
		close(this->getFd());
	}
}

void			User::setFd(int fd) { _sockfd = fd; }

int				User::getFd() { return(this->_sockfd); }

User::~User() { };