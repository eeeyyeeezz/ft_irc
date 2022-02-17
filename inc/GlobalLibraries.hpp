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

// ERRORS
#define NO_USER_TO_PRIVATEMSG send(_fd, "411 ERR_NORECIPIENT\n", 21, 0)
#define NO_SUCH_NICK send(_fd, "401 ERR_NOSUCHNICK\n", 20, 0)
#define NICK_NAME_IN_USE send(_fd, "433 ERR_NICKNAMEINUSE\n", 23, 0)
#define NEED_MORE_PARAMS send(server.getUser(i).getFd(), "461 ERR_NEEDMOREPARAMS\n", 24, 0)
#define CANNOT_SEND_TO_CHAN send(fd, "404 ERR_CANNOTSENDTOCHAN: ", 26, 0)

// NEW ERRORS
// COMMON ERRORS
#define ERR_NOTREGISTERED "451 *  :You have not registered\n"
#define ERR_UNKNOWNCOMMAND(command) ("421 *  " + command + " :Unknown command")
#define ERR_NEEDMOREPARAMS(command) ("461 *  " + command + " :Not enough parameters\n")

// PASS ERRORS
#define ERR_PASSWDMISMATCH "464 *  :Password incorrect\n"
// PASS and USER ERROR in case if user is already registered
#define ERR_ALREADYREGISTRED "462 *  :You may not reregister\n"

// NICK ERRORS
#define ERR_ERRONEUSNICKNAME(nick) ("432 *  " + nick + " :Erroneus nickname\n")
#define ERR_NICKNAMEINUSE(nick) ("433 *  " + nick + " :Nickname is already in use\n"

// PRIVMSG and NOTICE ERRORS
#define ERR_NORECIPIENT(command) ("411 * :No recipient given " + command + "\n")
#define ERR_NOTEXTTOSEND "412 *  :No text to send\n"
#define ERR_CANNOTSENDTOCHAN(channel) ("404 *  " + channel " :Cannot send to channel\n")
#define ERR_NOSUCHNICK(nick) ("401 *  " + nick + " :No such nick/channel\n")

// JOIN ERRORS
#define ERR_CHANNELISFULL(channel) ("471 *  " + channel + " :Cannot join channel (+l)")
#define ERR_NOSUCHCHANNEL(channel) ("403 *  " + channel + " :No such channel\n")
#define ERR_TOOMANYCHANNELS(channel) ("405 *  " + channel + " :You have joined too many channels\n")

// PART and KICK ERRORS
#define ERR_NOSUCHCHANNEL(channel) ("403 *  " + channel + " :No such channel\n")
#define ERR_NOTONCHANNEL(channel) ("442 *  ") + channel + " :You're not on that channel\n")
#define ERR_CHANOPRIVSNEEDED(channel) ("482 *  " + channel " :You're not channel operator\n")
// END OF NEW ERRORS

// GET_PASSED
#define NEW_USER(nickname, username) ("001 * Welcome to the Internet Relay Network " + nickname + "!" + username +"@127.0.0.1\r\n")
#define GET_USER_PASSED server.getUser(i).getUserPassed()
#define GET_NICK_PASSED server.getUser(i).getNickNamePassed()

// DEFINES
#define NEW_NICK_NAME_SET send(_fd, "New nickname set!\n", 19, 0)
#define NEED_NICK_OR_USER send(server.getUser(i).getFd(), "You need to write NICK or USER command with argument before you can chat\n", 74, 0)
#define NEW_USER_CREATED send(server.getUser(i).getFd(), "New user created!\n", 19, 0)
#define SEND_ABOUT_NEW_USER std::cout << WHITE << "NEW USER! NICKNAME: " << BLUE << "[" << server.getUser(i).getNickname() << "]" << WHITE << " USERNAME: "<< BLUE << "[" << server.getUser(i).getUsername() << "]" << WHITE << " FD " << BLUE << "[" << server.getUser(i).getFd() << "]" << NORMAL << std::endl;

void	SendMessageIrcSyntax(int fd, string nickname, string username, string message);
bool	contains(string array[], string message);
void	error(string error);