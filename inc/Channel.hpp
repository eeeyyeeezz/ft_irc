#pragma once
#include "GlobalLibraries.hpp"


class Channel{
	private:
	string				_channelName;
	vector<int>			_fds;
	int					_fdAmin;

	public:
	// Channel();
	Channel(string channelName);
	// ~Channel();

};

