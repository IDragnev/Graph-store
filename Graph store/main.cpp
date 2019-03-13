#include <iostream>
#include "Application\Application.h"
#include "String\String.h"
#include "UtilityFunctions.h"

using IDragnev::GraphStore::Application;
using IDragnev::Utility::print;

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		print(std::cerr, "Invalid number of arguments! Usage: ", argv[0], "\n");
		return 1;
	}

	Application::instance().run(argv[1]);

	return 0;
}