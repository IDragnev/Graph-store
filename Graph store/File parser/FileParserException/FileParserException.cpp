#include "FileParserException.h"


FileParserException::FileParserException(const char* message) :
	runtime_error(message)
{
}