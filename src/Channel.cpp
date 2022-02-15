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

void	NewUserConnect(Server &server, string message, string nickname, string username, int id){
	Channel tmpChannel = server.getChannel(id);
	vector<int> tmpFdVector = tmpChannel.getFdVector();
	
	for (int i = 0; i < tmpFdVector.size(); i++)
		SendMessageIrcSyntax(tmpFdVector[i], nickname, username, message);
	// :nickname!username@host message	
}

bool	checkChannelNameExist(vector<Channel> &tmpVector, string channelName){
	for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
		if ((*it).getChannelName() == channelName)
			return true;
	}
	return false;
}

bool Channel::checkUserInChannel(int fd) {
	if (fd == _fdAdmin) {
	  return true;
	}
	for (vector<int>::iterator it = _fds.begin(); it != _fds.end(); it++){
		if ((*it) == fd) {
		  return true;
		}
	}
	return false;
}

void	Channel::doChannelPrivmsg(int _fd, string message, string nickname, string username){
	if (checkUserInChannel(_fd)) {
		for (vector<int>::iterator it = _fds.begin(); it != _fds.end(); it++) {
		  if((*it) != _fd)
		  	SendMessageIrcSyntax((*it), nickname, username, message);
		}
	} else {
		CANNOT_SEND_TO_CHAN;
		send(_fd, _channelName.c_str(), _channelName.length() + 1, 0);
		send(_fd, "\n", 1, 0);
	}
}

void	Command::doJoinCommand(Server &server){
	static int channelID = 0;
	bool channelNameExist = false;
	
	vector<Channel> tmpVector = server.getVectorOfChannels();
	channelNameExist = checkChannelNameExist(tmpVector, _arguments[0]);
	
	if (!channelNameExist){
		Channel *channel = new Channel(_arguments[0], _fd);
		server.channelsPushBack(channel);
		server.setUsersAtChannelFd(channelID);
		NewUserConnect(server, _message, _nickname, _username, channelID);
		std::cout << "NEW CHANNEL! " << _arguments[0] << " ADMIN IS " << _nickname << std::endl;
		++channelID;
		return ;
	} else {
		// if not admin and not already in channel
		for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
			if ((*it).getChannelName() == _arguments[0]) {
				if ((*it).getFdAdmin() != _fd){
					vector<int>::iterator it2;
					vector<int> tmpFd = (*it).getFdVector();
					for (it2= tmpFd.begin(); it2 != tmpFd.end(); it2++){
						if ((*it2) == _fd)
							break;
					}
					if (it2 == tmpFd.end()) {
						server.channelPushBackFd(channelID - 1, _fd);
						NewUserConnect(server, _message, _nickname, _username, channelID - 1);
						std::cout << "NEW MEMBER AT " << server.getChannel(channelID - 1).getChannelName() << " BY FD " << _fd << " " << _nickname << std::endl;
					}
				}
			}
		}	
	}
}

void	Command::doPartCommand(Server &server){
	bool channelNameExist = false;
	vector<Channel> tmpVector = server.getVectorOfChannels();
	channelNameExist = checkChannelNameExist(tmpVector, _arguments[0]);
	
	// check if user in channel!
	
	
	if (!channelNameExist){
		// 403 ERR_NOSUCHCHANNEL
		std::cout << "NO.SUCH.CHANNEL\n";
		return ;
	} else {
		int atChannelFd = server.getUserAtChannelFd();
		Channel tmpChannel = server.getChannel(atChannelFd);
		vector<int> tmpIntFdsVector = server.getChannel(atChannelFd).getFdVector();
		vector<int>::iterator element = std::find(tmpIntFdsVector.begin(), tmpIntFdsVector.end(), _fd);
		if (element != tmpIntFdsVector.end()){
			// segfault
			int fdAdmin = server.getChannel(atChannelFd).getFdAdmin();
			tmpIntFdsVector.erase(element);
			tmpChannel.setFdVector(tmpIntFdsVector);
			// seg end

			if (_fd == fdAdmin){
				server.setNewChannelAdm(tmpIntFdsVector); // CHECK IF ADM!!
				std::cout << "NEW ADM SET\n";
			}
			server.channelSetNew(tmpChannel, atChannelFd);
			std::cout << _nickname << " LEAVES " << _arguments[0] << std::endl;;
			return ;
		} else { 
			// 442 ERR_NOTONCHANNEL
			std::cout << _nickname << " NOT IN CHANNNEL " << _arguments[0] << std::endl;
			return ;	
		}
	}
}
void Channel::doKickFromChannel(int fd, int userFd) {
  if(fd == _fdAdmin) {
	vector<int>::iterator itb = _fds.begin();
	vector<int>::iterator ite = _fds.end();
	for(vector<int>::iterator it = itb; it!=ite; it++) {
	  if((*it) == userFd) {
		_fds.erase(it);
	  }
	}
  } else
	std::cout << "You are not admin\n";
}
void Channel::printFds() {
  vector<int>::iterator itb = _fds.begin();
  vector<int>::iterator ite = _fds.end();
  for(vector<int>::iterator it = itb; it!=ite; it++) {
	std::cout << "fd" << (*it) << "\n";
	}
}
Channel::~Channel() { }