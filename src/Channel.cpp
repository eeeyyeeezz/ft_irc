#include "../inc/Channel.hpp"
#include "../inc/Command.hpp"

Channel::Channel() { }

Channel::Channel(string channelName, int fd) : _channelName(channelName), _fdAdmin(fd), _channelID(-1) { _fds.push_back(fd); }

// GETTERS
vector<int>			Channel::getFdVector(){ return(_fds); }
int					Channel::getFdAdmin(){ return(_fdAdmin); }
string				Channel::getChannelName(){ return(_channelName); }

// SETTERS
void				Channel::fdsPushBack(int fd) { _fds.push_back(fd); }
void				Channel::setChannelName(string channelName) { _channelName = channelName; }
void				Channel::setFdVector(vector<int> &fds) { _fds = fds; }
void				Channel::setFdAdmin(int fd) { _fdAdmin = fd; }
void				Channel::setNewVector(vector<int> &newVector) { _fds = newVector; }

void	NewUserConnect(Server &server, int fd, string message, string nickname, string username, int id, string channelName){
	Channel tmpChannel = server.getChannel(id);
	vector<int> tmpFdVector = tmpChannel.getFdVector();
	string userJoined = ":127.0.0.1 " + nickname + " " + "JOIN " + channelName + "\r\n";
	
	for (int i = 0; i < tmpFdVector.size(); i++){
		if (tmpFdVector[i] != fd)
			send(tmpFdVector[i], userJoined.c_str(), userJoined.length() + 1, 0);
	}
	
	string beginMessage = string(":KVIrc 331 " + nickname + " " + channelName + ": No topic is set\r\n"); // +  
	// ":KVIrc 353 " + nickname + " = " + channelName + " :@" + nickname + "\r\n"); 
	// ":KVIrc 366 " + nickname + " " + channelName + " :End of /NAMES list\r\n");
	send(fd, beginMessage.c_str(), beginMessage.length() + 1, 0);	
}

bool	checkChannelNameExist(vector<Channel> &tmpVector, string channelName){
	for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
		if ((*it).getChannelName() == channelName)
			return true;
	}
	return false;
}

bool Channel::checkUserInChannel(int fd) {
	if (fd == _fdAdmin)
		return true;
		
	for (vector<int>::iterator it = _fds.begin(); it != _fds.end(); it++){
		if ((*it) == fd)
			return true;
	}
	return false;
}

void	Channel::doChannelPrivmsg(int fd, string message, string nickname, string username){
	if (checkUserInChannel(fd)) {
		for (vector<int>::iterator it = _fds.begin(); it != _fds.end(); it++) {
			if ((*it) != fd)
				SendMessageIrcSyntax((*it), nickname, username, message);
		}
	} else {
		CANNOT_SEND_TO_CHAN;
		std::cout << "NOT.IN.CHANNEL!\n";
	}
}

void	Command::createNewChannel(Server &server){
	Channel *channel = new Channel(_arguments[0], _fd);
	int	_channelID = server.getChannelID();
	server.channelsPushBack(channel);
	delete channel;
	
	server.setUsersAtChannelFd(_channelID);
	NewUserConnect(server, _fd, _message, _nickname, _username, _channelID, _arguments[0]);
	std::cout << "NEW CHANNEL! " << _arguments[0] << " ADMIN IS " << _nickname << std::endl;
	server.setChannelID(1);
}

void	Command::doJoinCommand(Server &server){
	int	_channelID = server.getChannelID();
	bool channelNameExist = false;
	
	vector<Channel> tmpVector = server.getVectorOfChannels();
	channelNameExist = checkChannelNameExist(tmpVector, _arguments[0]);
	
	std::cout << "CHANNEL ID IS " << _channelID << std::endl;
	if (!channelNameExist)
		createNewChannel(server);
	else {
		// if not admin and not already in channel
		for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
			if ((*it).getChannelName() == _arguments[0]){
				if ((*it).getFdAdmin() != _fd){
					vector<int>::iterator it2;
					vector<int> tmpFd = (*it).getFdVector();
					for (it2= tmpFd.begin(); it2 != tmpFd.end(); it2++){
						if ((*it2) == _fd)
							break;
					}
					if (it2 == tmpFd.end()){
						server.channelPushBackFd(_channelID - 1, _fd);
						server.setUsersAtChannelFd(_channelID - 1);
						NewUserConnect(server, _fd, _message, _nickname, _username, _channelID - 1, _arguments[0]);
						std::cout << "NEW MEMBER AT " << server.getChannel(_channelID - 1).getChannelName() << " BY FD " << _fd << " " << _nickname << std::endl;
					}
				}
			}
		}
	}
}

void	partUser(Server &server, vector<int> &tmpIntFdsVector, Channel &tmpChannel, vector<int>::iterator &element, int atChannelFd, string nickname, string channelName, int fd){
	int fdAdmin = server.getChannel(atChannelFd).getFdAdmin();
	tmpIntFdsVector.erase(element);
	tmpChannel.setFdVector(tmpIntFdsVector);
	if (fd == fdAdmin && tmpIntFdsVector.size() > 0){
		server.setNewChannelAdm(tmpIntFdsVector);
		std::cout << "NEW ADM FD IS " << server.getChannel(server.getId()).getFdAdmin() << std::endl;
	}
	server.channelSetNew(tmpChannel, atChannelFd);
	
	// send to all that leaves
	string userLeaved = ":127.0.0.1 " + nickname + " " + "PART " + channelName + "\r\n";
	for (int i = 0; i < tmpIntFdsVector.size(); i++)
		send(tmpIntFdsVector[i], userLeaved.c_str(), userLeaved.length() + 1, 0);
	std::cout << nickname << " LEAVES " << channelName << std::endl;
	return ;
}

void	Command::doPartCommand(Server &server){
	bool channelNameExist = false;
	int	_channelID = server.getChannelID();
	vector<Channel> tmpVector = server.getVectorOfChannels();
	channelNameExist = checkChannelNameExist(tmpVector, _arguments[0]);
	
	if (!channelNameExist)
		send(_fd, ERR_NOSUCHCHANNEL(_arguments[0]).c_str(), ERR_NOSUCHCHANNEL(_arguments[0]).length() + 1, 0);
	else {
		int atChannelFd = server.getUserAtChannelFd();
		Channel tmpChannel = server.getChannel(atChannelFd);
		vector<int> tmpIntFdsVector = server.getChannel(atChannelFd).getFdVector();
		vector<int>::iterator element = std::find(tmpIntFdsVector.begin(), tmpIntFdsVector.end(), _fd);
		
		if (tmpChannel.getFdVector().size() == 1){
			tmpVector.erase(tmpVector.begin());
			server.channelVectorSetNew(tmpVector);
			server.setChannelID(-1);
			std::cout << "CHANNEL " << _arguments[0] << " DELETED\n";
			return ;
		}
		
		if (element != tmpIntFdsVector.end())
			partUser(server, tmpIntFdsVector, tmpChannel, element, atChannelFd, _nickname, _arguments[0], _fd);
		else
			send(_fd, ERR_NOTONCHANNEL(_arguments[0]).c_str(),  ERR_NOTONCHANNEL(_arguments[0]).length() + 1, 0);
	}
}

void Command::doKickCommand(Server &server) {
	if(_arguments.size() < 2){
		send(_fd, ERR_NEEDMOREPARAMS(string("KICK")).c_str(), ERR_NEEDMOREPARAMS(string("KICK")).length() + 1, 0);
		return;
	}

	vector<User> tmpVectorOfUsers = server.getVectorOfUsers();
	bool userExist = false;
	int userFd;
	for (vector<User>::iterator it = tmpVectorOfUsers.begin(); it != tmpVectorOfUsers.end(); it++){
		if ((*it).getNickname() == _arguments[1]){
			userExist = true;
			userFd = (*it).getFd();
			break;
		}
	}
	
	if (userExist){
		Channel tmpChannel;
		vector<Channel> tmpVector = server.getVectorOfChannelsRef();
		for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
			if ((*it).getChannelName() == _arguments[0]){
				(*it).doKickFromChannel(_fd, userFd);
				std::cout << _arguments[1] << " WAS KICKED FROM " << _arguments[0] << std::endl;
				server.channelVectorSetNew(tmpVector);
			}
		}
	}
}

void Channel::doKickFromChannel(int fd, int userFd){
	if (fd == _fdAdmin){
		vector<int>::iterator itb = _fds.begin();
		vector<int>::iterator ite = _fds.end();
		vector<int>::iterator it;
		for (it = itb; it != ite; it++){
			if ((*it) == userFd){
				_fds.erase(it);
				return;
			}
		}
		if (it == ite) ERR_USER_NOT_IN_CHANNEL;
	} else	ERR_CHAN_O_PRIVS_NEEDED;
}

void Channel::printFds() {
	vector<int>::iterator itb = _fds.begin();
	vector<int>::iterator ite = _fds.end();
	for (vector<int>::iterator it = itb; it != ite; it++){
		std::cout << "fd" << (*it) << "\n";
	}
}
Channel::~Channel() { }