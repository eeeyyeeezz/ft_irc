#pragma once

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>

using std::string;
using std::vector;

#define BUFFER_SIZE 4096

// COLORS
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define WHITE "\033[1;37m"
#define GREEN "\033[1;32m"
#define PURPLE "\033[1;35m"
#define GRAY "\033[1;30m"
#define NORMAL "\033[0m"

// NEW ERRORS
// COMMON ERRORS
#define ERR_NOTREGISTERED "451 *  :You have not registered\r\n"
#define ERR_UNKNOWNCOMMAND(command) ("421 *  " + command + " :Unknown command")
#define ERR_NEEDMOREPARAMS(command) ("461 *  " + command + " :Not enough parameters\n")

// PASS ERRORS
#define ERR_PASSWDMISMATCH "464 *  :Password incorrect\n"
// PASS and USER ERROR in case if user is already registered
#define ERR_ALREADYREGISTRED "462 *  :You may not reregister\n"

// NICK ERRORS
#define ERR_ERRONEUSNICKNAME(nick) ("432 *  " + nick + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(nick) ("433 *  " + nick + " :Nickname is already in use\r\n")

// PRIVMSG and NOTICE ERRORS
#define ERR_NORECIPIENT(command) ("411 * :No recipient given " + command + "\n")
#define ERR_NOTEXTTOSEND "412 *  :No text to send\n"
#define ERR_CANNOTSENDTOCHAN(channel) ("404 *  " + channel " :Cannot send to channel\n")
#define ERR_NOSUCHNICK(nick) ("401 *  " + nick + " :No such nick/channel\n")

// JOIN ERRORS
#define ERR_CHANNELISFULL(channel) ("471 *  " + channel + " :Cannot join channel (+l)\r\n")
// #define ERR_NOSUCHCHANNEL(channel) ("403 *  " + channel + " :No such channel\r\n")
#define ERR_TOOMANYCHANNELS(channel) ("405 *  " + channel + " :You have joined too many channels\r\n")
#define ERR_BADCHANNELKEY(channel) ("475 * " + channel + " :Cannot join channel (+k)\r\n")

// PART and KICK ERRORS
#define ERR_NOSUCHCHANNEL(channel) ("403 *  " + channel + " :No such channel\n")
#define ERR_NOTONCHANNEL(channel) ("442 *  " + channel + " :You're not on that channel\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) ("482 *  " + channel " :You're not channel operator\n")
#define	ERR_USER_NOT_IN_CHANNEL send(fd, "441 ERR_USERNOTINCHANNEL \r\n", 28, 0)
// END OF NEW ERRORS

// GET_PASSED
#define NEW_USER(nickname, username) ("001 * Welcome to the Internet Relay Network " + nickname + "!" + username +"@127.0.0.1\r\n")
#define GET_USER_PASSED server.getUser(i).getUserPassed()
#define GET_NICK_PASSED server.getUser(i).getNickNamePassed()

// DEFINES
#define SEND_ABOUT_NEW_USER std::cout << WHITE << "NEW USER! NICKNAME: " << BLUE << "[" << server.getUser(i).getNickname() << "]" << WHITE << " USERNAME: "<< BLUE << "[" << server.getUser(i).getUsername() << "]" << WHITE << " FD " << BLUE << "[" << server.getUser(i).getFd() << "]" << NORMAL << std::endl;

void	SendMessageIrcSyntax(int fd, string nickname, string username, string message);
void	error(string error);