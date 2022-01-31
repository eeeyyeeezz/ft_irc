#include "../inc/User.hpp"

User::User() { };

User::User(string message){
	string commands[2] = {"ADD", "KILL"};


	// std::cout << " IN HERE\n"; 
}


User::User(string username, string hostname, string servername, string realname) 
: _username(username), _hostname(hostname), _servername(servername), _realname(realname) {

}


void		User::parsCommand(){



}

User::~User() { };