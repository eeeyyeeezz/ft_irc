#pragma once
#include "GlobalLibraries.hpp"

class Command{
	private:
	string			_command;
	vector<string>	_arguments;

	public:
	Command();
	Command(string message);
	~Command();

	int		commandStart();
};


