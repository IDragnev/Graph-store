#include "ParseFail.h"
#include <string>


ParseFail::ParseFail(const String& filename, const String& reason, unsigned line) :
	Exception(buildMessage(filename, reason, line))
{
}


std::string ParseFail::buildMessage(const String& filename, const String& reason, unsigned line)
{
	std::string message = "Error reading " + filename;
	message += ": ";
	message += reason;
	message += ". Line " + std::to_string(line);

	return message;
}