#include "../inc/User.hpp"

User::User() { };

User::User(int fd) : _sockfd(fd), _passwordPassed(0), _nickNamePassed(0), _userPassed(0) {  }

User::User(string username, string hostname, string servername, string realname) 
: _username(username), _hostname(hostname), _servername(servername), _realname(realname) {

}

// SETTERS
void		User::setFd(int fd) { _sockfd = fd; }
void		User::setNickname(string nickname) { _nickname = nickname; }
void		User::setPasswordPassed() { _passwordPassed = 1; }
void		User::setNickName(string nickname){ _nickname = nickname; }

// GETTERS
int			User::getFd() { return(_sockfd); }
int			User::getPasswordPassed() { return(this->_passwordPassed); }
int			User::getUserPassed() { return(this->_userPassed); }
int			User::getNickNamePassed() { return(this->_nickNamePassed); }
bool		User::getAllPrepArguments() { return _passwordPassed && _nickNamePassed && _userPassed ? true : false;  }
string		User::getNickname() { return(_nickname); }

// PARSING

int			User::parsCommand(Server &server, string message, int i){
	bool allPrepIsDone = server.getUser(i).getAllPrepArguments();

	if (!allPrepIsDone)
		return server.getUser(i).preparationCommands(server, message, i);
	
	// all prep is done


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
	// if (server.getUser(i).getPasswordPassed() == 0 && firstWord == "PASS"){
	// 	server.getUser(i).checkUserPassword(server, message, i);
	// 	return (1);
	// } else if (server.getUser(i).getPasswordPassed() == 0 && firstWord != "PASS") {
	// 	send(server.getUser(i).getFd(), "You need to write PASS command and password\n", 45, 0);
	// 	return (1);
	// }

	// NICK / USER
	bool NickUserPassed = server.getUser(i).getUserPassed() && server.getUser(i).getNickNamePassed() ? true : false;
	if (!NickUserPassed){
		if (firstWord == "NICK\n" || firstWord == "NICK")
			return(server.getUser(i).parsNickCommand(server, message, i));
		else if (firstWord == "USER\n" || firstWord == "USER")
			return(server.getUser(i).parsUserCommand(server, message, i));
		else{
			send(server.getUser(i).getFd(), "You need to write NICK or USER command with argument before you can chat\n", 74, 0);
			return (1);
		}
	} 


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
	server.setUsernameByUser(parametr, i);

	std::cout << WHITE << server.getUser(i).getFd() << " NICK IS " << BLUE << "[" << server.getUser(i).getNickname() << "]" << NORMAL << std::endl;


	return (1);
}

int				User::parsUserCommand(Server &server, string message, int i){
	
	
	return (1);
}

void			User::checkUserPassword(Server &server, string message, int i){
	size_t findI = message.find(" ");
	while (message[findI] && message[findI] == ' ')
		findI++;
	if (!message[findI]){
		send(server.getUser(i).getFd(), "No arguments at PASS\n", 22, 0);
		close(server.getUser(i).getFd());
	}
	
	string parametr = message.substr(findI, message.length());
	parametr.erase(std::remove(parametr.begin(), parametr.end(), '\n'), parametr.end());

	if (parametr == server.getPassword()){
		send(server.getUser(i).getFd(), "Password correct!\n", 19, 0);
		// server.acceptedUsersPushBack(fd);
		server.setPasswordPassedByUser(i);
		std::cout << server.getUser(i).getPasswordPassed() << std::endl;
	} else {
		send(server.getUser(i).getFd(), "Password wrong!\n", 17, 0);
		close(server.getUser(i).getFd());
	}
}

User::~User() { };