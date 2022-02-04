#include "../inc/Command.hpp"

Command::Command() { }

Command::Command(string message, int fd, string nickname, vector<User> &users) : _fd(fd), _nickname(nickname), _users(users) {
	if (!message.empty()){	
		std::istringstream stringToSplit(message.c_str());
		string stringSplitted;
	
		while (getline(stringToSplit, stringSplitted, ' ' ) && stringSplitted != " ")
			_arguments.push_back(stringSplitted);
		_command = _arguments[0];
		_command.erase(std::remove(_command.begin(), _command.end(), '\n'), _command.end());
		
		// _command delete
		_arguments.erase(_arguments.begin());

		for (vector<string>::iterator it = _arguments.begin(); it != _arguments.end(); it++)
			(*it).erase(std::remove((*it).begin(), (*it).end(), '\n'), (*it).end());
	}
}

// GETTERS
vector<User>		Command::getVectorOfUsers() { return (_users); }


int		Command::commandStart(Server &server){
	string	commands[] = {"NICK", "QUIT", "PRIVMSG", "NOTICE", "HELP", "JOIN", "PART", "KICK", "BOT"};
	if (std::find(std::begin(commands), std::end(commands), _command) != std::end(commands)){
		checkCommand(server);
		return (1);
	}

	return (0);
}

void	Command::checkCommand(Server &server){
	
	if (_command == "QUIT") doQuitCommand();
	else if (_command == "NICK") doNickCommand(server);
	else if (_command == "PRIVMSG") doPrivmsgCommand(1);
	else if (_command == "NOTICE") doNoticeCommand();
	else if (_command == "JOIN") doJoinCommand(server);

	// BOT commands
	if (_command == "BOT" && _arguments[0] == "HELP") doHelpCommand();
}

void	Command::doJoinCommand(Server &server){
	// Channel *channel = new Channel();
	
	// server.channelsPushBack(channel);
}

void	Command::doHelpCommand(){
	send(_fd, "COMMANDS:\n", 10, 0);
	send(_fd, "NICK\n", 5, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   NICK <nickname>\n", 19, 0);
	send(_fd, "   Allows a client to change their IRC nickname.\n", 49, 0);
	send(_fd, "QUIT\n", 5, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   QUIT [<message>]\n", 20, 0);
	send(_fd, "   Disconnects the user from the server.\n", 41, 0);
	send(_fd, "JOIN\n", 5, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   JOIN <channels> [<keys>]\n", 28, 0);
	send(_fd, "   Makes the client join the channels in the comma-separated list <channels>, specifying the passwords, if needed, in the comma-separated list <keys>. If the channel(s) do not exist then they will be created.\n", 209, 0);
	send(_fd, "PRIVMSG\n", 8, 0);
	send(_fd, "   Syntax:\n", 11,	0);	
	send(_fd, "   PRIVMSG <msgtarget> :<message>\n", 34, 0);
	send(_fd, "   Sends <message> to <msgtarget>, which is usually a user or channel.\n", 71, 0);
	send(_fd, "NOTICE\n", 7, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   NOTICE <msgtarget> <message>\n", 32, 0);
	send(_fd, "   This command works similarly to PRIVMSG, except automatic replies must never be sent in reply to NOTICE messages.\n", 117, 0);
	send(_fd, "PART\n", 5, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   PART <channels> [<message>]\n", 31, 0);
	send(_fd, "   Sets a connection password. This command must be sent before the NICK/USER registration combination.\n", 104, 0);
	send(_fd, "KICK\n", 5, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   KICK <channel> <client> :[<message>]\n", 40, 0);
	send(_fd, "   Forcibly removes <client> from <channel>. This command may only be issued by channel operators.\n", 99, 0);
	send(_fd, "BOT\n", 4, 0);
	send(_fd, "   Syntax:\n", 11, 0);
	send(_fd, "   BOT HELP\n", 12, 0);
	send(_fd, "   Рelps clients navigate in commands\n", 38, 0);
}

void	Command::doNoticeCommand() { doPrivmsgCommand(0); }

void	Command::doQuitCommand(){
	string goodbyeMessage;
	for (vector<string>::iterator it = _arguments.begin(); it != _arguments.end(); it++)
		goodbyeMessage += *it + " ";
	
	std::cout << RED << _fd << ":" << BLUE << " [" << goodbyeMessage << "]" << NORMAL;
	close(_fd);
}

void	Command::doNickCommand(Server &server){
	string newNick = _arguments[0];
	newNick.erase(std::remove(newNick.begin(), newNick.end(), '\n'), newNick.end());
	for (vector<User>::iterator it = server.getVectorOfUsers().begin(); it != server.getVectorOfUsers().end(); it++){
		if ((*it).getNickname() == newNick){
			NICK_NAME_IN_USE;
			return ;
		}
	}
	server.setNicknameByUser(newNick, server.getId());
	// _nickname = _arguments[0];		// Ne tak nado u usera menyat'
	NEW_NICK_NAME_SET;
}

void	Command::doPrivmsgCommand(int flag){
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
	for (int i = 1; i < _arguments.size(); i++)
		privateMessage += _arguments[i] + " ";
	// privateMessage.erase(std::remove(privateMessage.end(), privateMessage.end(), ' '), privateMessage.end());
	// privateMessage.erase(std::remove(privateMessage.begin(), privateMessage.end(), '\n'), privateMessage.end());

	if (userExist){
		if (flag)
			send(fdToPm, "PM from ", 9, 0);
		else
			send(fdToPm, "NOTICE ", 8, 0);
		send(fdToPm, _nickname.c_str(), _nickname.length(), 0);
		send(fdToPm, ": ", 2, 0);
		send(fdToPm, privateMessage.c_str(), privateMessage.length(), 0);
		// std::cout << "USER EXIST AT PRIVMSG!\n";
	}
	else {
		NO_SUCH_NICK;
		return ;
	}
	if (flag)
		send(_fd, "Message send!\n", 15, 0);

}


Command::~Command() { }
