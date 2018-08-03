#include "GraphBuilderException.h"


GraphBuilderException::GraphBuilderException(const char* message) :
	runtime_error(message)
{
}


GraphBuilderException::GraphBuilderException(const std::string& message) :
	runtime_error(message)
{
}