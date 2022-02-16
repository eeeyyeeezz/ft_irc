#include "../inc/Command.hpp"
#include <chrono>
#include <ctime>

void	Command::doShowtimeCommand(){
	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	string timeNow = "Time is " + std::string(std::ctime(&end_time));
	SendMessageIrcSyntax(_fd, "Bot", "◑.◑", timeNow);
}

void	Command::doRandnumberCommand(){
	if (_arguments.size() != 3){
		SendMessageIrcSyntax(_fd, "Bot", "◑.◑", "Syntax is: BOT RANDNUMBER <first int> <second int>");
		return ;
	}
	try{
		int firstNum = stoi(_arguments[1]);
		int secondNum = stoi(_arguments[2]);
		int randNum = rand() % secondNum + firstNum;
		string randNumberToShow = "Rand number is [" + std::to_string(randNum) + "]";
		SendMessageIrcSyntax(_fd, "Bot", "◑.◑", randNumberToShow);
	} catch(std::invalid_argument& e) { SendMessageIrcSyntax(_fd, "Bot", "◑.◑", "Send integer!"); }
}

void	Command::doHelpCommand(){
	if (_arguments.size() == 1)
		SendMessageIrcSyntax(_fd, "Bot", "◑.◑", "BOT commands: HELP, SHOWTIME, RANDNUMBER, ");
	else
		SendMessageIrcSyntax(_fd, "Bot", "◑.◑", "Wrong BOT syntax");
}