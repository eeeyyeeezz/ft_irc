#include "../inc/Command.hpp"



void	Command::doShowtimeCommand(){
	
}

void	Command::doRandnumberCommand(){
	
}

void	Command::doHelpCommand(){
	if (_arguments.size() == 2 + 1){
		if (_arguments[1] == "NICK"){
		send(_fd, "NICK\n", 6, 0);
		send(_fd, "   Syntax:\n", 12, 0);
		send(_fd, "   NICK <nickname>\n", 20, 0);
		send(_fd, "   Allows a client to change their IRC nickname.\n", 50, 0);
		} else if (_arguments[1] == "QUIT"){
		send(_fd, "QUIT\n", 6, 0);
		send(_fd, "   Syntax:\n", 12, 0);
		send(_fd, "   QUIT [<message>]\n", 21, 0);
		send(_fd, "   Disconnects the user from the server.\n", 42, 0);
		} else if (_arguments[1] == "JOIN"){
		send(_fd, "JOIN\n", 5, 0);
		send(_fd, "   Syntax:\n", 11, 0);
		send(_fd, "   JOIN <channels> [<keys>]\n", 28, 0);
		send(_fd, "   Makes the client join the channels in the comma-separated list <channels>, specifying the passwords, if needed, in the comma-separated list <keys>. If the channel(s) do not exist then they will be created.\n", 209, 0);
		} else if (_arguments[1] == "PRIVMSG"){
		send(_fd, "PRIVMSG\n", 8, 0);
		send(_fd, "   Syntax:\n", 11,	0);	
		send(_fd, "   PRIVMSG <msgtarget> :<message>\n", 34, 0);
		send(_fd, "   Sends <message> to <msgtarget>, which is usually a user or channel.\n", 71, 0);
		} else if (_arguments[1] == "NOTICE"){
		send(_fd, "NOTICE\n", 7, 0);
		send(_fd, "   Syntax:\n", 11, 0);
		send(_fd, "   NOTICE <msgtarget> <message>\n", 32, 0);
		send(_fd, "   This command works similarly to PRIVMSG, except automatic replies must never be sent in reply to NOTICE messages.\n", 117, 0);
		} else if (_arguments[1] == "PART"){
		send(_fd, "PART\n", 5, 0);
		send(_fd, "   Syntax:\n", 11, 0);
		send(_fd, "   PART <channels> [<message>]\n", 31, 0);
		send(_fd, "   Sets a connection password. This command must be sent before the NICK/USER registration combination.\n", 104, 0);
		} else if (_arguments[1] == "KICK"){
		send(_fd, "KICK\n", 5, 0);
		send(_fd, "   Syntax:\n", 11, 0);
		send(_fd, "   KICK <channel> <client> :[<message>]\n", 40, 0);
		send(_fd, "   Forcibly removes <client> from <channel>. This command may only be issued by channel operators.\n", 99, 0);
		}
	} else if (_arguments.size() == 1 + 1){
		send(_fd, "   BOT HELP <command>\n", 23, 0);
		send(_fd, "   Рelps clients navigate in commands\n", 40, 0);
		send(_fd, "BOT commands: HELP, SHOWTIME, RANDNUMBER, \n", 32, 0);
	} else 
		send(_fd, "Wrong BOT arguments\n", 21, 0);
		// send(_fd, "Syntax: BOT <command>\n", 23, 0);
}