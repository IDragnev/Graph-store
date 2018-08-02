#include "FileParserException.h"


FileParserException::FileParserException(const char* message) :
	runtime_error(message)
{
}


FileParserException::FileParserException(const std::string& message) :
	runtime_error(message)
{
}