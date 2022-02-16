#include "../inc/Server.hpp"
#include "../inc/User.hpp"

#define BUFFER_SIZE 4096

Server::Server(int port, string password) : _port(port), _password(password), _countConnects(0) { }

// GETTERS
int				Server::getId() { return (_id); }
int				Server::getPort() { return(_port); }
void			Server::setListening(int socket) { _listening = socket; }
int				Server::getListening() { return(_listening); }
int				Server::getCountConnects() { return(_countConnects); }
vector<Channel>	Server::getVectorOfChannels() { return(_channels); }
vector<User>	Server::getVectorOfUsers() { return(_users); }
Channel			Server::getChannel(int i) { return(_channels[i]); }
string			Server::getPassword() { return(_password); }

int				Server::getUserAtChannelFd() { return(_users[_id].getAtChannelFd()); }
User			Server::getUser(int i) { return(_users[i]); }

// SETTERS
void			Server::setPasswordPassedByUser(int i) { _users[i].setPasswordPassed(); }
void			Server::setNicknamePassedByUser(int i) { _users[i].setNicknamePassed(); }
void			Server::setUserPassedByUser(int i) { _users[i].setUserPassed(); }
void			Server::setId(int id) { _id = id;  }

void			Server::setCountConnects(int i) { _countConnects += i; }
void			Server::setUsernameByUser(string username, int i) {  _users[i].setUsername(username); }
void			Server::setNicknameByUser(string nickname, int i) { _users[i].setNickname(nickname); }
void			Server::channelsPushBack(Channel *channel) { _channels.push_back(*channel); }

void			Server::userPushBack(User *user) { _users.push_back(*user); }
void			Server::usersVectorSetNew(vector<User> &tmpVector) { _users = tmpVector; }

void			Server::setUsersAtChannelFd(int fd) { _users[_id].setAtChannelFd(fd); }
void			Server::setNewChannelAdm(vector<int> &tmpIntFdsVector) { _channels[_id].setFdAdmin(tmpIntFdsVector[0]); }
void			Server::channelPushBackFd(int id, int fd) { _channels[id].fdsPushBack(fd); }
void			Server::channelVectorSetNew(vector<Channel> &tmpVector){ _channels = tmpVector; }
void			Server::channelSetNew(Channel &newChannel, int id) { _channels[id] = newChannel; }

// SERVER
void	Server::createSocket(Server &server){
	server.setListening(socket(AF_INET, SOCK_STREAM, 0));
	if (server.getListening() == -1) { error("Error establishing connection"); } 
}

void	Server::bindSocket(Server &server){
	int flagOne = 1;
	if (setsockopt(server.getListening(), SOL_SOCKET, SO_REUSEADDR, &flagOne, sizeof(int)) < 0){ error("setsockpt error"); }

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(server.getPort());
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
	
	if (bind(server.getListening(), (sockaddr *)&hint, sizeof(hint)) == -1) { error("Can't bind"); } 
}

void	Server::listenSocket(Server &server, struct pollfd fds[]){
	if (listen(server.getListening(), SOMAXCONN) == -1) { error("Can't listen"); } 
	fds[0].fd = server.getListening();
	fcntl(fds[0].fd, F_SETFL, O_NONBLOCK);
}

void	Server::writeToServerAndAllUsers(string buff, int readed, struct pollfd fds[], int i){
	std::cout << getUser(i - 1).getNickname() << ": " << buff;

}

void	Server::mainLoop(Server &server, struct pollfd fds[]){
	int flag = 0;
	std::cout << "Server start!\n";
	while (true){
		int COUNTFD;
		
		if (flag > 0) { std::cout << "Exit\n" ; exit(EXIT_SUCCESS); } // exit_success
		if ((COUNTFD = poll(fds, server.getCountConnects(), -1)) < 0) { error("Poll crash"); } // do_error poll crash

		for (size_t i = 0; i < server.getCountConnects(); i++){
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN) == POLLIN){
				++flag;
				if (i == 0) 
					server.setNewConnection(flag, fds, i);
				else
					server.continueConnection(flag, fds, i);
			}
		}
	}
}

void	Server::setNewConnection(int &flag, struct pollfd fds[], size_t &i){
	User *user = new User(fds[i].fd);
	_users.push_back(*user);

	flag = 0;
	fds[getCountConnects()].fd = accept(fds[i].fd, NULL, NULL);
	std::cout << YELLOW << "NEW CONNNECT" << NORMAL << std::endl;
	fds[getCountConnects()].events = POLLIN;
	fds[getCountConnects()].revents = 0;
	setCountConnects(1);
}

void	Server::continueConnection(int &flag, struct pollfd fds[], size_t &i){
	char buff[BUFFER_SIZE];
	flag = 0;
	
	memset(buff, 0, BUFFER_SIZE);
	int readed = read(fds[i].fd, buff, BUFFER_SIZE);
	fds[i].revents = 0;
	if (!readed){
		std::cout << RED << fds[i].fd << BLUE << "  disconnected" << NORMAL << std::endl;
		fds[i].fd = -1;
		_users.erase(_users.begin() + i - 1);
		setCountConnects(-1);
	}
	buff[readed] = 0;
	_users[i - 1].setFd(fds[i].fd);
	setId(i - 1);
	_users[i].parsCommand(*this, std::string(buff), i - 1, fds);
	writeToServerAndAllUsers(std::string(buff), readed, fds, i);
	fds[i].revents = 0;
}

Server::~Server(){ }