#include "../inc/Command.hpp"
#include <chrono>
#include <ctime>

void	Command::doHelpCommand(){
	if (_arguments.size() == 1)
		send(_fd, "BOT COMMANDS: HELP, SHOWTIME, RANDNUMBER, INFO, SHOWUSER\r\n", 59, 0);
	else
		send(_fd, "BOT ERROR Wrong syntax\r\n", 25, 0);
}

void	Command::doInfoCommand(){
	string showInfo = ": BOT NICKNAME is [" + _nickname + "]\r\n:BOT USERNAME IS [" + _username + "]\r\n";
	send(_fd, showInfo.c_str(), showInfo.length() + 1, 0);
}

void	Command::doShowuserCommand(Server &server){
	if (_arguments.size() != 2){
		send(_fd, ":BOT Syntax is: BOT SHOWUSER <id>\r\n", 36, 0);
		return ;
	}
	
	try{
		int id = stoi(_arguments[1]);
		vector<User> tmpVectorOfUsers = server.getVectorOfUsers();

		string infoToSend;
		int numberOfUsers = tmpVectorOfUsers.size();
		string stringNumberOfUsers = ":BOT SHOWUSERS NUMBER OF USERS [" + std::to_string(numberOfUsers) + "]\r\n";
		if (id > numberOfUsers - 1 || id < 0){
			send(_fd, ":BOT Out of range!\r\n", 21, 0);
			throw std::invalid_argument("Out of range");
		}

		string infoAboutUserID = ":BOT USER ID [" + std::to_string(id) + "]\r\n";
		string infoAboutUserUsername = ":BOT USER USERNAME [" + tmpVectorOfUsers[id].getUsername() + "]\r\n";
		string infoAboutUserNickname = ":BOT USER NICKNAME [" + tmpVectorOfUsers[id].getNickname() + "]\r\n";
		infoToSend = stringNumberOfUsers + infoAboutUserID + infoAboutUserUsername + infoAboutUserNickname;
		send(_fd, infoToSend.c_str(), infoToSend.length() + 1, 0);
	} catch(std::invalid_argument& e) { send(_fd, "BOT Send integer!\r\n", 20, 0); }
	
}

void	Command::doShowtimeCommand(){
	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	string timeNow = ":BOT Time is " + std::string(std::ctime(&end_time));
	send(_fd, timeNow.c_str(), timeNow.length() + 1, 0);
}

void	Command::doRandnumberCommand(){
	if (_arguments.size() != 3){
		send(_fd, ":Syntax is: BOT RANDNUMBER <first int> <second int>\r\n", 54, 0);
		return ;
	}
	try{
		int firstNum = stoi(_arguments[1]);
		int secondNum = stoi(_arguments[2]);
		int randNum = rand() % secondNum + firstNum;
		string randNumberToShow = ":BOT Rand number is [" + std::to_string(randNum) + "]\r\n";
		send(_fd, randNumberToShow.c_str(), randNumberToShow.length() + 1, 0);
	} catch(std::invalid_argument& e) { send(_fd, "BOT Send integer!\r\n", 20, 0); }
}