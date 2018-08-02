#include "GraphException.h"


GraphException::GraphException(const char* message) :
	std::runtime_error(message)
{
}


GraphException::GraphException(const std::string& message) :
	std::runtime_error(message)
{
}