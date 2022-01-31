#include "../inc/Server.hpp"
#include <iostream>
using std::string;

void	error(string error){
	std::cout << error << std::endl;
	exit(EXIT_FAILURE);
}