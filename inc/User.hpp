#ifndef USER_HPP
# define USER_HPP

#include <iostream>
using std::string;

class User {

	private:
		string username;
		string hostname;		// не проверяется 
		string servername;		// не проверяется 
		string realname;
		string password;
		string nickname;
		int	sockfd;
		int	port;

		User();
		User (User const & );
		User & operator = (User const & );

	public:
		User(string username, string hostname, string servername, string realname);
		~User();
	
	

};

#endif