#include "GraphException.h"


GraphException::GraphException(const char* message) :
	std::runtime_error(message)
{
}