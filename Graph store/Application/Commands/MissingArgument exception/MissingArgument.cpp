#include "MissingArgument.h"


MissingArgument::MissingArgument(const std::string& name) :
	Exception{ "Missing argument: [" + name + "]" }
{
}