#include "../inc/User.hpp"

User::User() :  _passwordPassed(0), _nickNamePassed(0), _userPassed(0) { };

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
void		startDebug(Server &server){
	{
	server.setNicknameByUser("gmorra", 0);
	server.setUsernameByUser("0", 0);
	server.setPasswordPassedByUser(0);
	server.setUserPassedByUser(0);
	server.setNicknamePassedByUser(0);
	std::cout << WHITE << "NEW USER! NICKNAME: " << BLUE << "[" << server.getUser(0).getNickname() << "]" << WHITE << " USERNAME: "<< BLUE << "[" << server.getUser(0).getUsername() << "]" << NORMAL << std::endl;
	}

	{
	// User *user = new User();
	// server.userPushBack(user);
	// server.getUser(1).setFd(5);	// ?
	server.setNicknameByUser("mhogg", 1);
	server.setUsernameByUser("1", 1);
	server.setPasswordPassedByUser(1);
	server.setUserPassedByUser(1);
	server.setNicknamePassedByUser(1);
	std::cout << WHITE << "NEW USER! NICKNAME: " << BLUE << "[" << server.getUser(1).getNickname() << "]" << WHITE << " USERNAME: "<< BLUE << "[" << server.getUser(1).getUsername() << "]" << NORMAL << std::endl;;
	}


}


int			User::parsCommand(Server &server, string message, int i){
	static int onlyOnce = 0;
	bool allPrepIsDone = server.getUser(i).getAllPrepArguments();

	// if (!allPrepIsDone)
	// 	return server.getUser(i).preparationCommands(server, message, i);
	if (!onlyOnce){
		startDebug(server);
		++onlyOnce;
	}
	// all prep is done
	
	vector<User> newVector = server.getVectorOfUsers();
	Command command(message, server.getUser(i).getFd(), server.getUser(i).getNickname(), newVector);
	return command.commandStart(server);
}
	
int			setFindI(Server &server, string message, int i){
	size_t findI;
	if (message.find(" ") != message.npos)
		findI = message.find(" ");
	else{
		NEED_MORE_PARAMS; 
		return -1;
	}
	while (message[findI] && message[findI] == ' ')
		findI++;
	return findI;
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
		else{
			NEED_NICK_OR_USER;
			return (1);
		}
	} 

	SEND_ABOUT_NEW_USER;
	return (0);
}

int				User::parsNickCommand(Server &server, string message, int i){
	size_t findI;
	if ((findI = setFindI(server, message, i)) == -1)
		return (1);
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());
	if (parametr == ""){
		NEED_MORE_PARAMS;
		return (1);
	}
	send(server.getUser(i).getFd(), "Nickname set!\n", 15, 0);
	server.setNicknameByUser(parametr, i);
	server.setNicknamePassedByUser(i);
	if (GET_USER_PASSED){
		NEW_USER_CREATED;
		server.acceptedUsersPushBack(server.getUser(i).getFd());
		SEND_ABOUT_NEW_USER;
	}
	return (1);
}

int				User::parsUserCommand(Server &server, string message, int i){
	size_t	findI;
	if ((findI = setFindI(server, message, i)) == -1)
		return (1);
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());
	if (parametr == ""){
		NEED_MORE_PARAMS;
		return (1);
	}
	send(server.getUser(i).getFd(), "Username set!\n", 15, 0);
	server.setUsernameByUser(parametr, i);
	server.setUserPassedByUser(i);
	if (GET_NICK_PASSED){
		NEW_USER_CREATED;
		server.acceptedUsersPushBack(server.getUser(i).getFd());
		SEND_ABOUT_NEW_USER;
	}
	return (1);
}

void			User::checkUserPassword(Server &server, string message, int i){
	size_t findI;
	if ((findI = setFindI(server, message, i)) == -1)
		return ;
		
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());

	if (parametr == server.getPassword()){
		send(server.getUser(i).getFd(), "Password correct!\n", 19, 0);
		server.setPasswordPassedByUser(i);
	} else{
		PASSWORD_WRONG;
		close(server.getUser(i).getFd());
	}
}

User::~User() { };