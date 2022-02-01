#include "../inc/User.hpp"

User::User() { };

User::User(int fd) : _sockfd(fd), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) { }

User::User(string password) : _password(password), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) { }

User::User(string username, string hostname, string servername, string realname) 
: _username(username), _hostname(hostname), _servername(servername), _realname(realname) {

}

int			User::parsCommand(Server &server, string message, int fd, int i){
	// bool allPrepIsDone = 
	string firstWord = message.substr(0, message.find(" "));

	if (server._users[i]._passwordPassed == 0 && firstWord == "PASS"){
		server._users[i].checkUserPassword(server, message, fd, i);
		return (1);
	} else if (server._users[i]._passwordPassed == 0 && firstWord != "PASS") {
		send(fd, "You need to write PASS command and password\n", 45, 0);
		return (1);
	}



	return (0);
}

void			User::checkUserPassword(Server &server, string message, int fd, int i){
	size_t findI = message.find(" ");
	while (message[findI] && message[findI] == ' ')
		findI++;
	if (!message[findI]){
		send(fd, "No arguments at PASS\n", 22, 0);
		close(fd);
	}
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());

	if (parametr == server.getPassword()){
		send(fd, "Password correct!\n", 19, 0);
		server.acceptedUsersPushBack(fd);
		server._users[i]._passwordPassed = 1;
	} else {
		send(fd, "Password wrong!\n", 17, 0);
		close(fd);
	}
}

void			User::setFd(int fd) { _sockfd = fd; }

int				User::getFd() { return(this->_sockfd); }

User::~User() { };