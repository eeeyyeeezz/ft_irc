#include "../inc/User.hpp"

User::User() { };

User::User(int fd) : _sockfd(fd), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) {  }


// SETTERS
void		User::setFd(int fd) { _sockfd = fd; }
void		User::setUsername(string username) { _username = username; }
void		User::setNickname(string nickname) { _nickname = nickname; }
void		User::setPasswordPassed() { _passwordPassed = 1; }
void		User::setNicknamePassed() { _nickNamePassed = 1; }
void		User::setUserPassed() { _userPassed = 1; }


// GETTERS
int			User::getFd() { return(_sockfd); }
int			User::getPasswordPassed() { return(this->_passwordPassed); }
int			User::getUserPassed() { return(this->_userPassed); }
int			User::getNickNamePassed() { return(this->_nickNamePassed); }
bool		User::getAllPrepArguments() { return _passwordPassed && _nickNamePassed && _userPassed ? true : false; }
string		User::getUsername() { return(_username); }
string		User::getNickname() { return(_nickname); }

// PARSING
int			User::parsCommand(Server &server, string message, int i){
	bool allPrepIsDone = server.getUser(i).getAllPrepArguments();

	// if (!allPrepIsDone)
	// 	return server.getUser(i).preparationCommands(server, message, i);
	
	// all prep is done
	string firstWord;
	if (message.find(" ") != message.npos)
		firstWord = message.substr(0, message.find(" "));
	else
		firstWord = message.substr(0, message.length() - 1);
	
	// PASS 
	if (firstWord == "QUIT"){
		return(1);

	}
	// if(firstWord)
	return (0);
}

int			User::preparationCommands(Server &server, string message, int i){
	string firstWord;
	if (message.find(" ") != message.npos)
		firstWord = message.substr(0, message.find(" "));
	else
		firstWord = message.substr(0, message.length() - 1);

	// PASS 
	if (server.getUser(i).getPasswordPassed() == 0 && firstWord == "PASS"){
		server.getUser(i).checkUserPassword(server, message, i);
		return (1);
	} else if (server.getUser(i).getPasswordPassed() == 0 && firstWord != "PASS") {
		send(server.getUser(i).getFd(), "You need to write PASS command and password\n", 45, 0);
		return (1);
	}

	// NICK / USER
	bool NickUserPassed = server.getUser(i).getUserPassed() && server.getUser(i).getNickNamePassed() ? true : false;
	if (!NickUserPassed){
		if (firstWord == "NICK\n" || firstWord == "NICK")
			return(server.getUser(i).parsNickCommand(server, message, i));
		else if (firstWord == "USER\n" || firstWord == "USER")
			return(server.getUser(i).parsUserCommand(server, message, i));
		else {
			send(server.getUser(i).getFd(), "You need to write NICK or USER command with argument before you can chat\n", 74, 0);
			return (1);
		}
	} 

	SEND_ABOUT_NEW_USER;
	return (0);
}

int				User::parsNickCommand(Server &server, string message, int i){
	size_t findI;
	if (message.find(" ") != message.npos)
		findI = message.find(" ");
	else{
		send(server.getUser(i).getFd(), "No arguments at NICK [1]\n", 26, 0);
		return (1);
	} 

	while (message[findI] && message[findI] == ' ')
		findI++;
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());
	if (parametr == ""){
		send(server.getUser(i).getFd(), "No arguments at NICK [2]\n", 26, 0);
		return (1);
	}
	send(server.getUser(i).getFd(), "Nickname set!\n", 15, 0);
	server.setNicknameByUser(parametr, i);
	server.setNicknamePassedByUser(i);
	if (GET_USER_PASSED)
		SEND_ABOUT_NEW_USER;
	return (1);
}

int				User::parsUserCommand(Server &server, string message, int i){
	size_t findI;
	if (message.find(" ") != message.npos)
		findI = message.find(" ");
	else{
		send(server.getUser(i).getFd(), "No arguments at USER [1]\n", 26, 0);
		return (1);
	} 

	while (message[findI] && message[findI] == ' ')
		findI++;
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());
	if (parametr == ""){
		send(server.getUser(i).getFd(), "No arguments at USER [2]\n", 26, 0);
		return (1);
	}
	send(server.getUser(i).getFd(), "Username set!\n", 15, 0);
	server.setUsernameByUser(parametr, i);
	server.setUserPassedByUser(i);
	if (GET_NICK_PASSED)
		SEND_ABOUT_NEW_USER;
	return (1);
}

void			User::checkUserPassword(Server &server, string message, int i){
	size_t findI;
	if (message.find(" ") != message.npos)
		findI = message.find(" ");
	else{
		send(server.getUser(i).getFd(), "No arguments at PASS\n", 22, 0);
		return ;
	} 
	while (message[findI] && message[findI] == ' ')
		findI++;
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());

	if (parametr == server.getPassword()){
		send(server.getUser(i).getFd(), "Password correct!\n", 19, 0);
		// server.acceptedUsersPushBack(fd);
		server.setPasswordPassedByUser(i);
	} else {
		send(server.getUser(i).getFd(), "Password wrong!\n", 17, 0);
		close(server.getUser(i).getFd());
	}
}

User::~User() { };