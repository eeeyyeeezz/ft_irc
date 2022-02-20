#include "../inc/GlobalLibraries.hpp"

// Выводим у тех, кто получил сообщение: ”:Nick!<username>@host Command <args>”
// например “:user1!John@127.0.0.1 PRIVMSG user2 :Hello\r\n”

void	SendMessageIrcSyntax(int fd, string nickname, string username, string message){
	string messageToSend = ":" + nickname + "!" + username + "@127.0.0.1 " + message + "\r\n";
	send(fd, messageToSend.c_str(), messageToSend.length() + 1, 0);
}

void	error(string error){
	std::cout << error << std::endl;
	exit(EXIT_FAILURE);
}