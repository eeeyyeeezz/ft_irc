#pragma once
#include "GlobalLibraries.hpp"


class Channel{
	private:
	string				_channelName;
	vector<int>			_fds;
	int					_channelID;
	int					_fdAdmin;

	public:
	Channel();
	Channel(string channelName, int fd);
	~Channel();

	// GETTERS
	string				getChannelName();
	vector<int>			getFdVector();
	int					getFdAdmin();

	// SETTERS
	void				fdsPushBack(int fd);
	bool 				doKickFromChannel(int fd, int userFd);
	void				doChannelPrivmsg(int fd, string message, string nickname, string username);
	void				setChannelName(string channelName);
	void				setFdVector(vector<int> &fds);
	void				setFdAdmin(int fd);
	
	void				setNewVector(vector<int> &newVector);
	bool				checkUserInChannel(int fd);

	//check
	void				printFds();

};

