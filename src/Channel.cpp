#include "../inc/Channel.hpp"

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


Channel::~Channel() { }