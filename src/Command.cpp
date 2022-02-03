#include "../inc/Command.hpp"

bool	contains(string array[], string message){
	for (int i = 0; i < array->length(); i++){
		if (array[i] == message)
			return (true);
	}
	return (false);
}


Command::Command() { }

Command::Command(string message, int fd, string nickname, vector<User> users) : _fd(fd), _nickname(nickname), _users(users) {
	if (!message.empty()){	
  		std::istringstream stringToSplit(message.c_str());
  		string stringSplitted;
	
		while (getline(stringToSplit, stringSplitted, ' ' ) && stringSplitted != " ")
			_arguments.push_back(stringSplitted);
		_command = _arguments[0];
		_arguments.erase(_arguments.begin());
		// std::cout << "COMMAND : " << _command << std::endl;
		// for (vector<string>::iterator it = _arguments.begin(); it != _arguments.end(); it++)
		// 	std::cout << "ARGS: " << *it << std::endl;
	}
}

int		Command::commandStart(){
	string	commands[] = {"NICK", "QUIT", "JOIN", "PRIVMSG", "NOTICE", "PART", "KICK", "HELP"};
	if (contains(commands, _command)){
		checkCommand();
		return (1);
	}

	return (0);
}

void	Command::checkCommand(){
	if (_command == "QUIT") doQuitCommand();
	else if (_command == "NICK") doNickCommand();
	
}

void	Command::doQuitCommand(){
	std::cout << RED << _nickname << BLUE << "  disconnected" << NORMAL << std::endl;;
	string goodbyeMessage;
	for (vector<string>::iterator it = _arguments.begin(); it != _arguments.end(); it++)
		goodbyeMessage += *it + " ";
	
	// std::cout << RED << _fd << ":" << BLUE << " [" << goodbyeMessage << "]" << NORMAL;
	close(_fd);
}

void	Command::doNickCommand(){
	// for (vector<User>::iterator it = _users.begin(); it != _users.end(); it++){
	// 	if (*it == _arguments[0]){
	// 		NICK_NAME_IN_USE;
	// 		return ;
	// 	}
	// }
	// _nickname = _arguments[0];
}

Command::~Command() { }
