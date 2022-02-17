#pragma once
#include "User.hpp"
#include "GlobalLibraries.hpp"
#include "Command.hpp"
#include "Channel.hpp"

class User;

class Server{
	private:
	int				_port;
	int				_id;
	int				_channelID;	// just iterator for channels
	int				_listening;
	int				_countConnects;
	string			_password;

	vector<User>	_users;
	vector<Channel>	_channels;

	public:
	Server();
	Server(int port, string password);
	~Server();

	// GETTERS
	int				getId();
	int				getChannelID();
	int				getPort();
	int				getListening();
	int				getCountConnects();
	vector<Channel>	getVectorOfChannels();
  	vector<Channel>	&getVectorOfChannelsRef();
	vector<User>	getVectorOfUsers();
	
	User			getUser(int i);
	int				getUserAtChannelFd();
	
	Channel			getChannel(int i);
	string			getPassword();
	
	// SETTERS
	void			setUsernameByUser(string username, int i);
	void			setNicknameByUser(string username, int i);

	void			setPasswordPassedByUser(int i);
	void			setNicknamePassedByUser(int i);
	void			setUserPassedByUser(int i);

	void			setChannelID(int id);
	void			setId(int id);
	void			setListening(int socket);
	void			setCountConnects(int i);
	
	void			userPushBack(User *user);
	void			usersVectorSetNew(vector<User> &tmpVector);
	
	void			setUsersAtChannelFd(int fd);
	void			channelsPushBack(Channel *channel);
	void			channelSetNew(Channel &newChannel, int id);
	void			channelVectorSetNew(vector<Channel> &tmpVector);
	void			channelPushBackFd(int id, int fd);
	
	// SERVER
	void			setNewChannelAdm(vector<int> &fdVector);
	
	void			createSocket(Server &server);
	void			bindSocket(Server &server);
	void			listenSocket(Server &server, struct pollfd fds[]);
	void			mainLoop(Server &server, struct pollfd fds[]);
	void			setNewConnection(int &flag, struct pollfd fds[], size_t &i);
	void			continueConnection(int &flag, struct pollfd fds[], size_t &i);
	
};


void	error(string error);