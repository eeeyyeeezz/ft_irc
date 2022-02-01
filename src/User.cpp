#include "../inc/User.hpp"

User::User() { };

User::User(int fd) : _sockfd(fd), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) {  }

User::User(string password) : _password(password), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) { }

User::User(string username, string hostname, string servername, string realname) 
: _username(username), _hostname(hostname), _servername(servername), _realname(realname) {

}

// SETTERS
void		User::setFd(int fd) { _sockfd = fd; }
void		User::setPasswordPassed() { _passwordPassed = 1; }
void		User::setNickName(string nickname){ _nickname = nickname; }

// GETTERS
int			User::getFd() { return(_sockfd); }
bool		User::getAllPrepArguments() { return _passwordPassed && _nickNamePassed && _userPassed ? true : false;  }

// PARSING

int			User::parsCommand(Server &server, string message, int i){
	bool allPrepIsDone = server.getUser(i).getAllPrepArguments();

	if (!allPrepIsDone)
		return server.getUser(i).preparationCommands(server, message, i);
	//


	if(firstWord)
	return (0);
}

int			User::preparationCommands(Server &server, string message, int i){
	string firstWord = message.substr(0, message.find(" "));
	if (server.getUser(i)._passwordPassed == 0 && firstWord == "PASS"){
		server.getUser(i).checkUserPassword(server, message, server.getUser(i).getFd(), i);
		return (1);
	} else if (server.getUser(i)._passwordPassed == 0 && firstWord != "PASS") {
		send(server.getUser(i).getFd(), "You need to write PASS command and password\n", 45, 0);
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
		server.setPasswordPassedByUser(i);
	} else {
		send(fd, "Password wrong!\n", 17, 0);
		close(fd);
	}
}

User::~User() { };