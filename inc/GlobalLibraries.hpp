#pragma once

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <iostream>
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
#define NICK_NAME_IN_USE send(_fd, "433 ERR_NICKNAMEINUSE\n", 23, 0)
#define NEED_MORE_PARAMS send(server.getUser(i).getFd(), "461 ERR_NEEDMOREPARAMS\n", 24, 0)
#define PASSWORD_WRONG send(server.getUser(i).getFd(), "464 ERR_PASSWDMISMATCH\n", 24, 0)

// GET_PASSED
#define GET_USER_PASSED server.getUser(i).getUserPassed()
#define GET_NICK_PASSED server.getUser(i).getNickNamePassed()

// DEFINES
#define NEW_NICK_NAME_SET send(_fd, "New nickname set!\n", 19, 0)
#define NEED_NICK_OR_USER send(server.getUser(i).getFd(), "You need to write NICK or USER command with argument before you can chat\n", 74, 0)
#define NEW_USER_CREATED send(server.getUser(i).getFd(), "New user created!\n", 19, 0)
#define SEND_ABOUT_NEW_USER std::cout << WHITE << "NEW USER! NICKNAME: " << BLUE << "[" << server.getUser(i).getNickname() << "]" << WHITE << " USERNAME: "<< BLUE << "[" << server.getUser(i).getUsername() << "]" << NORMAL << std::endl;

bool	contains(string array[], string message);
void	error(string error);