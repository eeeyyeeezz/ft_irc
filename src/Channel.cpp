#include "../inc/Channel.hpp"
#include "../inc/Command.hpp"

Channel::Channel() { }

Channel::Channel(string channelName, int fd) : _channelName(channelName), _fdAmin(fd), _channelID(-1) { _fds.push_back(fd); }

// GETTERS
vector<int>			Channel::getFdVector(){ return(_fds); }
int					Channel::getFdAdmin(){ return(_fdAmin); }
string				Channel::getChannelName(){ return(_channelName); }


// SETTERS
void				Channel::fdsPushBack(int fd) { _fds.push_back(fd); }
void				Channel::setChannelName(string channelName) { _channelName = channelName; }
void				Channel::setFdVector(vector<int> fds) { _fds = fds; }
void				Channel::setFdAdmin(int fd) { _fdAmin = fd; }

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

bool Command::checkUserInChannel(Channel &channel) {
	if(channel.getFdAdmin() == _fd)
		return true;
	vector<int> tmpVector = channel.getFdVector();
	for(vector<int>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++) {
		if((*it) == _fd)
			return true;
	}
	return false;
}

void	Command::doChannelPrivmsg(Channel &tmpChannel, int _fd, string message){
	if (checkUserInChannel(tmpChannel)) {
		vector<int> tmpFds = tmpChannel.getFdVector();
		for (vector<int>::iterator it = tmpFds.begin(); it != tmpFds.end(); it++)
			SendMessageIrcSyntax((*it), _nickname, _username, message);
	} else {
		CANNOT_SEND_TO_CHAN;
		send(_fd, tmpChannel.getChannelName().c_str(), tmpChannel.getChannelName().length() + 1, 0);
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
		for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++) {
			if ((*it).getChannelName() == _arguments[0]) {
				if((*it).getFdAdmin() != _fd) {
					vector<int>::iterator it2;
					vector<int> tmpFd = (*it).getFdVector();
					for(it2= tmpFd.begin(); it2 != tmpFd.end(); it2++) {
						if((*it2) == _fd) {
							break;
						}
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
	vector<int> tmpIntFdsVector = server.getChannel(server.getId()).getFdVector(); 
	channelNameExist = checkChannelNameExist(tmpVector, _arguments[0]);
	
	if (!channelNameExist){
		// 403 ERR_NOSUCHCHANNEL
		return ;
	} else {
		if (int element = std::find(tmpIntFdsVector.begin(), tmpIntFdsVector.end(), _fd) != tmpIntFdsVector.end()){
			// CHECK IF ADM!!
			
			tmpVector.erase(tmpVector.begin() + element);
			server.channelSetNew(tmpVector);
			std::cout << _nickname << " LEAVES " << _arguments[0];
			return ;
		}
		else { 
			// 442 ERR_NOTONCHANNEL
			return ;	
		}
	}
}

void	Server::channelSetNew(vector<Channel> &tmpVector){ _channels = tmpVector; }

Channel::~Channel() { }