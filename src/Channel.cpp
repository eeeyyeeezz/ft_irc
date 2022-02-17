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
	
	for (int i = 0; i < tmpFdVector.size(); i++){
		if (tmpFdVector[i] != fd)
			SendMessageIrcSyntax(tmpFdVector[i], nickname, username, message);
	}
	
	string beginMessage = string(":KVIrc 331 " + nickname + " " + channelName + ": No topis is set\r\n"); // +  
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
						NewUserConnect(server, _fd, _message, _nickname, _username, _channelID - 1, _arguments[0]);
						std::cout << "NEW MEMBER AT " << server.getChannel(_channelID - 1).getChannelName() << " BY FD " << _fd << " " << _nickname << std::endl;
					}
				}
			}
		}
	}
}

void	Command::doPartCommand(Server &server){
	bool channelNameExist = false;
	int	_channelID = server.getChannelID();
	vector<Channel> tmpVector = server.getVectorOfChannels();
	channelNameExist = checkChannelNameExist(tmpVector, _arguments[0]);
	
	if (!channelNameExist){
		// 403 ERR_NOSUCHCHANNEL
		std::cout << "NO.SUCH.CHANNEL\n";
		return ;
	} else {
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
		
		if (element != tmpIntFdsVector.end()){
			int fdAdmin = server.getChannel(atChannelFd).getFdAdmin();
			tmpIntFdsVector.erase(element);
			tmpChannel.setFdVector(tmpIntFdsVector);

			if (_fd == fdAdmin && tmpIntFdsVector.size() > 0){
				server.setNewChannelAdm(tmpIntFdsVector);
				std::cout << "NEW ADM FD IS " << server.getChannel(server.getId()).getFdAdmin() << std::endl;
			}
			server.channelSetNew(tmpChannel, atChannelFd);
			
			// send to all that leaves
			for (int i = 0; i < tmpIntFdsVector.size(); i++)
				SendMessageIrcSyntax(tmpIntFdsVector[i], _nickname, _username, _message);
			std::cout << _nickname << " LEAVES " << _arguments[0] << std::endl;;
			return ;
		} else { 
			// 442 ERR_NOTONCHANNEL
			std::cout << _nickname << " NOT IN CHANNNEL " << _arguments[0] << std::endl;
			return ;	
		}
	}
}

void Command::doKickCommand(Server &server) {
	if(_arguments.size() < 2){
		// 461 ERR_NEEDMOREPARAMS
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
		bool channelExist = false;
		Channel tmpChannel;
		vector<Channel> tmpVector = server.getVectorOfChannels();
		for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
			if ((*it).getChannelName() == _arguments[0]){
				channelExist = true;
				(*it).doKickFromChannel(_fd, userFd);
				server.channelVectorSetNew(tmpVector);
			}
		}
	}
}

void Channel::doKickFromChannel(int fd, int userFd){
	if (fd == _fdAdmin){
		vector<int>::iterator itb = _fds.begin();
		vector<int>::iterator ite = _fds.end();
		for (vector<int>::iterator it = itb; it != ite; it++){
			if ((*it) == userFd){
				_fds.erase(it);
			}
		}
	} else
		; // 482 ERR_CHANOPRIVSNEEDED
}

void Channel::printFds() {
	vector<int>::iterator itb = _fds.begin();
	vector<int>::iterator ite = _fds.end();
	for (vector<int>::iterator it = itb; it != ite; it++){
		std::cout << "fd" << (*it) << "\n";
	}
}
Channel::~Channel() { }