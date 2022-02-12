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

void	NewUserConnect(Server &server, string message){
	// Когда новый пользователь присоединяется к каналу, всем  участникам канала выводим сообщение: 
	// 	”:<nickname>!<username>@host Command <args>”
	// 		например “Jo!John@10.21.34.86 JOIN #chat”
	Channel tmpChannel = server.getChannel(server.getId());
	vector<int> tmpFdVector = tmpChannel.getFdVector();
	string nickname = server.getUser(server.getId()).getNickname();
	string username = server.getUser(server.getId()).getUsername();
	
	std::cout << "SIZE " << tmpFdVector.size() << std::endl;
	for (int i = 0; i < tmpFdVector.size(); i++){
		send(tmpFdVector[i], ":", 1, 0);
		send(tmpFdVector[i], nickname.c_str(), nickname.length() + 1, 0);
		send(tmpFdVector[i], "!", 1, 0);
		send(tmpFdVector[i], username.c_str(), nickname.length() + 1, 0);
		send(tmpFdVector[i], "@127.0.0.1 ", 12, 0);
		send(tmpFdVector[i], message.c_str(), message.length() + 1, 0);
	}
	// :nickname!username@host message	
}

void	Command::doJoinCommand(Server &server){
	static int channelID = 0;
	bool channelNameExist = false;
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
		server.setUsersAtChannelFd(channelID);
		NewUserConnect(server, _message);
		std::cout << "NEW CHANNEL! " << _arguments[0] << std::endl;
		++channelID;
		return ;
	} else {
		// if not admin and not already in channel
		for (vector<Channel>::iterator it = tmpVector.begin(); it != tmpVector.end(); it++) {
			if ((*it).getChannelName() == _arguments[0]) {
				if((*it).getFdAdmin() != _fd) {
					vector<int>::iterator it2;
					for(it2= (*it).getFdVector().begin(); it2 != (*it).getFdVector().end(); it2++) {
						if((*it2) == _fd) {
							break;
						}
					}
					if (it2 == (*it).getFdVector().end()) {
						server.getChannel(server.getId()).fdsPushBack(_fd);
						// NewUserConnect(server, _message);
						std::cout << "NEW MEMBER AT " << server.getChannel(server.getId()).getChannelName() << " BY FD " << _fd << std::endl;
					}
				}
			}
		}	
	}
}

Channel::~Channel() { }