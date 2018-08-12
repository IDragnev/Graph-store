#include "StringReader.h"


void StringReader::operator()(const std::string& name, const std::string& value, String& destination) const
{
	char buffer[BUFFER_SIZE]{ '\0' };

	size_t len = value.length();
	for (size_t i = 0; i < len; i++)
	{
		buffer[i] = value[i];
	}

	destination = buffer;
}