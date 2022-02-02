#include "../inc/Command.hpp"

Command::Command() { }

Command::Command(string message, int fd, string nickname, vector<User> &users) : _fd(fd), _nickname(nickname), _users(users) {
	// std::cout << "0 " << _users[0].getUsername() << " " << _users[0].getNickname() << std::endl;
	// std::cout << "1 " << _users[1].getUsername() << " " << _users[1].getNickname() << std::endl;

	// for (vector<User>::iterator it = _users.begin(); it != _users.end(); it++)
	// 	std::cout << (*it).getUsername() << std::endl;

	if (!message.empty()){	
		std::istringstream stringToSplit(message.c_str());
		string stringSplitted;
	
		while (getline(stringToSplit, stringSplitted, ' ' ) && stringSplitted != " ")
			_arguments.push_back(stringSplitted);
		_command = _arguments[0];
		_arguments.erase(_arguments.begin());
	}
}

// GETTERS
vector<User>		Command::getVectorOfUsers() { return (_users); }


int		Command::commandStart(){
	string	commands[] = {"NICK", "QUIT", "JOIN", "PRIVMSG", "NOTICE", "PART", "KICK", "HELP", "BOT"};
	if (contains(commands, _command)){
		checkCommand();
		return (1);
	}

	return (0);
}

void	Command::checkCommand(){
	if (_command == "QUIT") doQuitCommand();
	else if (_command == "NICK") doNickCommand();
	else if (_command == "PRIVMSG") doPrivmsgCommand();
	
}

void	Command::doQuitCommand(){
	string goodbyeMessage;
	for (vector<string>::iterator it = _arguments.begin(); it != _arguments.end(); it++)
		goodbyeMessage += *it + " ";
	
	std::cout << RED << _fd << ":" << BLUE << " [" << goodbyeMessage << "]" << NORMAL;
	close(_fd);
}

void	Command::doNickCommand(){
	for (vector<User>::iterator it = _users.begin(); it != _users.end(); it++){
		std::cout << "ASDIHASOIDHAOISDH\n";
		if ((*it).getNickname() == _arguments[0]){
			NICK_NAME_IN_USE;
			return ;
		}
	}
	_nickname = _arguments[0];		// Ne tak nado u usera menyat'
	NEW_NICK_NAME_SET;
}

void	Command::doPrivmsgCommand(){
	if (_arguments.size() < 2){
		 NO_USER_TO_PRIVATEMSG;
		 return ;
	}
	
	// if exist
	bool userExist = false;
	int fdToPm;
	// string userFromPm;
	for(vector<User>::iterator it = _users.begin(); it != _users.end(); it++){
		if ((*it).getNickname() == _arguments[0]){
			userExist = true;
			fdToPm = (*it).getFd();
			break;
		}
	}
	
	// arguments to count and ++ to write
	string privateMessage;

	if (userExist){
		send(fdToPm, "PM from ", 9, 0);
		send(fdToPm, _nickname.c_str(), _nickname.length(), 0);
		send(fdToPm, ": ", 2, 0);
		send(fdToPm, _arguments[1].c_str(), _arguments[1].length(), 0);
		// std::cout << "USER EXIST AT PRIVMSG!\n";
	}
	else {
		NO_SUCH_NICK;
		return ;
	}

}


Command::~Command() { }
