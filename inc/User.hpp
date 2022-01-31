#ifndef USER_HPP
# define USER_HPP

#include <iostream>
using std::string;

class User {

	private:
		string _username;
		string _hostname;		// не проверяется 
		string _servername;		// не проверяется 
		string _realname;
		string _password;
		string _nickname;
		int	_sockfd;
		int	_port;

		User();
		User (User const & );
		User & operator = (User const & );

	public:
		User(string message);
		User(string username, string hostname, string servername, string realname);
		~User();
	
	void	parsCommand();

};

#endif