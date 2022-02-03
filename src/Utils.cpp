#include "../inc/GlobalLibraries.hpp"

bool	contains(string array[], string message){
	for (int i = 0; i < array->length(); i++){
		if (array[i] == message)
			return (true);
	}
	return (false);
}

void	error(string error){
	std::cout << error << std::endl;
	exit(EXIT_FAILURE);
}