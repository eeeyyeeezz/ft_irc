#include "../inc/Command.hpp"

bool	contains(string array[], string message){
	for (int i = 0; i != array->length(); i++){
		if (array[i] == message)
			return (1);
	}
	return (0);
}


Command::Command() { }

Command::Command(string message){	
  	std::istringstream stringToSplit(message.c_str());
  	string stringSplitted;

	while (getline(stringToSplit, stringSplitted, ' ' ) && stringSplitted != " ")
		_arguments.push_back(stringSplitted);
	_command = _arguments[0];
	_arguments.erase(_arguments.begin());

	std::cout << "COMMAND : " << _command << std::endl;
	for (vector<string>::iterator it = _arguments.begin(); it != _arguments.end(); it++)
		std::cout << "ARGS: " << *it << std::endl;  

}

int		Command::commandStart(){
	string	commands[] = {"NICK", "QUIT", "JOIN", "PRIVMSG"};
	if (contains(commands, _command)){
		std::cout << "COMMAND IS " << _command << std::endl;
		return (1);
	}

	return (0);
}

Command::~Command() { }
