#include "../inc/Command.hpp"

Command::Command() { }

Command::Command(string message, int fd, string nickname, string username, vector<User> &users) : _message(message), _fd(fd), _nickname(nickname), _username(username), _users(users) {
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
			(*it).erase(std::remove((*it).begin(), (*it).end(), '\r'), (*it).end());

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
	
	if (_command == "QUIT") doQuitCommand(server);
	else if (_command == "NICK") doNickCommand(server);
	else if (_command == "PRIVMSG") doPrivmsgCommand(server);
	else if (_command == "NOTICE") doNoticeCommand(server);
	else if (_command == "JOIN") doJoinCommand(server);
	else if (_command == "PART") doPartCommand(server);
	else if (_command == "KICK") doKickCommand(server);

	// BOT commands
	if (_command == "BOT" && _arguments[0] == "HELP") doHelpCommand();
	if (_command == "BOT" && _arguments[0] == "SHOWTIME") doShowtimeCommand();
	if (_command == "BOT" && _arguments[0] == "RANDNUMBER") doRandnumberCommand();
}

void	Command::doNoticeCommand(Server &server) { doPrivmsgCommand(server); }

void	Command::doQuitCommand(Server &server){
	vector<User> tmpUser = server.getVectorOfUsers();
	vector<User>::iterator it_begin = tmpUser.begin();
	vector<User>::iterator it_end = tmpUser.end();
	
	for (; it_begin != it_end; it_begin++){
		if ((*it_begin).getFd() != _fd)
			send((*it_begin).getFd(), _message.c_str(), _message.length() + 1, 0);
	}
	
	close(_fd);
}

void	Command::doNickCommand(Server &server){
	string newNick = _arguments[0];
	newNick.erase(std::remove(newNick.begin(), newNick.end(), '\n'), newNick.end());
	vector<User> tmpVector = server.getVectorOfUsers();
	
	for (vector<User>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
		if ((*it).getNickname() == newNick){
			NICK_NAME_IN_USE;
			return ;
		}
	}
	server.setNicknameByUser(newNick, server.getId());
	NEW_NICK_NAME_SET;
}


void	Command::doPrivmsgCommand(Server &server){
	if (_arguments.size() < 2){ NO_USER_TO_PRIVATEMSG; return ; }
	
	// if exist
	bool userExist = false;
	int fdToPm;
	for (vector<User>::iterator it = _users.begin(); it != _users.end(); it++){
		if ((*it).getNickname() == _arguments[0]){
			userExist = true;
			fdToPm = (*it).getFd();
			break;
		}
	}
	
	bool channelExist = false;
	Channel tmpChannel;
	vector<Channel> tmpVector = server.getVectorOfChannels();
	for(vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
		if ((*it).getChannelName() == _arguments[0]){
			channelExist = true;
			tmpChannel = (*it);
			break;
		}
	}

	if (userExist)
		SendMessageIrcSyntax(fdToPm, _nickname, _username, _message);
	else if (channelExist) 
		tmpChannel.doChannelPrivmsg(_fd, _message, _nickname, _username);
	else {
		NO_SUCH_NICK;
		return ;
	}
}

Command::~Command() { }
