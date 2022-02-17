#include "../inc/Command.hpp"
#include <chrono>
#include <ctime>

void	Command::doHelpCommand(){
	if (_arguments.size() == 1)
		send(_fd, "BOT COMMANDS: HELP, SHOWTIME, RANDNUMBER, INFO, SHOWUSERS\r\n", 60, 0);
	else
		send(_fd, "BOT ERROR Wrong syntax\r\n", 25, 0);
}

void	Command::doInfoCommand(Command &user){
	string showInfo = "BOT NICKNAME is [" + _nickname + "]\r\nUSERNAME IS [" + _username + "]\r\n";
	send(_fd, showInfo.c_str(), showInfo.length() + 1, 0);
}

void	Command::doShowusersCommand(Server &server){
	vector<User> tmpVectorOfUsers = server.getVectorOfUsers();
	int numberOfUsers = tmpVectorOfUsers.size();
	
	string stringNumberOfUsers = "BOT SHOWUSERS NUMBER OF USERS [" + std::to_string(numberOfUsers) + "]\r\n";
	send(_fd, stringNumberOfUsers.c_str(), stringNumberOfUsers.length() + 1, 0);
	
	vector<User>::iterator it_begin = tmpVectorOfUsers.begin();
	vector<User>::iterator it_end = tmpVectorOfUsers.end();
	
	int i = 0;
	while (it_begin != it_end){
		string infoAboutUserID = "USER ID [" + std::to_string(i) + "]\r\n";
		string infoAboutUserNickname = "USER NICKNAME [" + string((*it_begin).getNickname()) + "]\r\n";
		string infoAboutUserUsername = "USER USERNAME [" + string((*it_begin).getUsername()) + "]\r\n";
		send(_fd, infoAboutUserID.c_str(), infoAboutUserID.length() + 1, 0);
		send(_fd, infoAboutUserUsername.c_str(), infoAboutUserID.length() + 1, 0);
		send(_fd, infoAboutUserNickname.c_str(), infoAboutUserID.length() + 1, 0);
		++it_begin;
		++i;
	}
}

void	Command::doShowtimeCommand(){
	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	string timeNow = "BOT Time is " + std::string(std::ctime(&end_time));
	send(_fd, timeNow.c_str(), timeNow.length() + 1, 0);
}

void	Command::doRandnumberCommand(){
	if (_arguments.size() != 3){
		send(_fd, "Syntax is: BOT RANDNUMBER <first int> <second int>\r\n", 53, 0);
		return ;
	}
	try{
		int firstNum = stoi(_arguments[1]);
		int secondNum = stoi(_arguments[2]);
		int randNum = rand() % secondNum + firstNum;
		string randNumberToShow = "BOT Rand number is [" + std::to_string(randNum) + "]\r\n";
		send(_fd, randNumberToShow.c_str(), randNumberToShow.length() + 1, 0);
	} catch(std::invalid_argument& e) { send(_fd, "BOT Send integer!\r\n", 20, 0); }
}