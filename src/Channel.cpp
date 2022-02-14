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
	// Когда новый пользователь присоединяется к каналу, всем  участникам канала выводим сообщение: 
	// 	”:<nickname>!<username>@host Command <args>”
	// 		например “Jo!John@10.21.34.86 JOIN #chat”
	Channel tmpChannel = server.getChannel(id);
	vector<int> tmpFdVector = tmpChannel.getFdVector();
	
	for (int i = 0; i < tmpFdVector.size(); i++)
		SendMessageIrcSyntax(tmpFdVector[i], nickname, username, message);
	// :nickname!username@host message	
}

void	Command::doChannelPrivmsg(Channel &tmpChannel, int _fd, string message){
	if (checkUserInChannel(tmpChannel)) {
		vector<int> tmpFds = tmpChannel.getFdVector();
		for (vector<int>::iterator it = tmpFds.begin(); it != tmpFds.end(); it++)
			SendMessageIrcSyntax((*it), _nickname, _username, message);
	} else {
		CANNOT_SEND_TO_CHAN;
		send(_fd, tmpChannel.getChannelName().c_str(), tmpChannel.getChannelName().length(), 0);
		send(_fd, "\n", 1, 0);
	}
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
		
		NewUserConnect(server, _message, _nickname, _username, channelID);
		std::cout << "NEW CHANNEL! " << _arguments[0] << " ADMIN IS " << _nickname << std::endl;
		++channelID;
		return ;
						// server.getChannel(server.getId()).fdsPushBack(_fd);
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

Channel::~Channel() { }