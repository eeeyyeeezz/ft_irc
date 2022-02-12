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
	int				_listening;
	int				_countConnects;
	string			_password;

	vector<int>		_acceptedUsers;
	vector<User>	_users;
	vector<Channel>	_channels;

	public:
	Server(int port, string password);
	~Server();

	// GETTERS
	int				getId();
	int				getPort();
	int				getListening();
	int				getCountConnects();
	vector<Channel>	getVectorOfChannels();
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

	void			channelsPushBack(Channel *channel);
	void			setId(int id);
	void			setListening(int socket);
	void			setCountConnects(int i);
	
	void			setUsersAtChannelFd(int fd);
	void			userPushBack(User *user);
	
	void			channelPushBackFd(int id, int fd);
	
	// SERVER
	void			createSocket(Server &server);
	void			bindSocket(Server &server);
	void			listenSocket(Server &server, struct pollfd fds[]);
	void			mainLoop(Server &server, struct pollfd fds[]);
	void			setNewConnection(int &flag, struct pollfd fds[], size_t &i);
	void			continueConnection(int &flag, struct pollfd fds[], size_t &i);
	
	// OTHER
	void			writeToServerAndAllUsers(string buff, int readed, struct pollfd fds[], int i);
	void			acceptedUsersPushBack(int value);
};


void	error(string error);