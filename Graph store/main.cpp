#include <iostream>
#include "Application\Application.h"
#include "String\String.h"

namespace GS = IDragnev::GraphStore;

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Invalid number of arguments! Usage: " << argv[0] << std::endl;
		return 1;
	}

	GS::Application::instance().run(argv[1]);

	return 0;
}