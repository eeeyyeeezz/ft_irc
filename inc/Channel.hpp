#pragma once
#include "GlobalLibraries.hpp"


class Channel{
	private:
	string				_channelName;
	vector<int>			_fds;
	int					_channelID;
	int					_fdAmin;

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
	void				setChannelName(string channelName);
	void				setFdVector(vector<int> fds);
	void				setFdAdmin(int fd);


};

