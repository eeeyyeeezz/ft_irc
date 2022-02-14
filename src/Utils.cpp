#include "../inc/GlobalLibraries.hpp"

// Выводим у тех, кто получил сообщение: ”:Nick!<username>@host Command <args>”
// например “:user1!John@127.0.0.1 PRIVMSG user2 :Hello”

void	SendMessageIrcSyntax(int fd, string nickname, string username, string message){
	send(fd, ":", 1, 0);
	send(fd, nickname.c_str(), nickname.length() + 1, 0);
	send(fd, "!", 1, 0);
	send(fd, username.c_str(), username.length() + 1, 0);
	send(fd, "@127.0.0.1 ", 12, 0);
	send(fd, message.c_str(), message.length() + 1, 0);
}

bool	contains(string array[], string message){
	for (int i = 0; i < array->length(); i++){
		if (array[i] == message) {
			std::cout << message << std::endl;
			return (true);
		}
	}
	return (false);
}

void	error(string error){
	std::cout << error << std::endl;
	exit(EXIT_FAILURE);
}