#include "../inc/Channel.hpp"
#include "../inc/Command.hpp"

Channel::Channel() { }

Channel::Channel(string channelName, int fd) : _channelName(channelName), _fdAmin(fd) { }

// GETTERS
vector<int>			Channel::getFdVector(){ return(_fds); }
int					Channel::getFdAdmin(){ return(_fdAmin); }
string				Channel::getChannelName(){ return(_channelName); }


// SETTERS
void				Channel::fdsPushBack(int fd) { _fds.push_back(fd); }
void				Channel::setChannelName(string channelName) { _channelName = channelName; }
void				Channel::setFdVector(vector<int> fds) { _fds = fds; }
void				Channel::setFdAdmin(int fd) { _fdAmin = fd; }

void	NewUserConnect(){
	
}

void	Command::doJoinCommand(Server &server){
	bool channelNameExist = true;
	vector<Channel> tmpVector = server.getVectorOfChannels();
	for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++){
		if ((*it).getChannelName() == _arguments[0]){
			channelNameExist = true;
			break ;
		}
	}
	
	if (!channelNameExist){
		Channel *channel = new Channel(_arguments[0], _fd);
		server.channelsPushBack(channel);
		std::cout << "NEW CHANNEL! " << _arguments[0] << std::endl;
		return ;
	} else {
		// if not admin and not already in channel
		for(vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++) {
			if ((*it).getChannelName() == _arguments[0]) {
				if((*it).getFdAdmin() != _fd) {
					vector<int>::iterator it2;
					for(it2= (*it).getFdVector().begin(); it2 != (*it).getFdVector().end(); it2++) {
						if((*it2) == _fd) {
							break;
						}
					}
					if(it2 == (*it).getFdVector().end()) {
						server.getChannel(server.getId()).fdsPushBack(_fd);
						std::cout << "NEW MEMBER AT " << server.getChannel(server.getId()).getChannelName() << " BY FD " << _fd << std::endl;
					}
				}
			}
		}	
	}
}

Channel::~Channel() { }