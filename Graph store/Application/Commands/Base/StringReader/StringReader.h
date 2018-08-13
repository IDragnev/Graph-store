#ifndef __STRING_READER_H_INCLUDED__
#define __STRING_READER_H_INCLUDED__

#include <string>
#include "..\..\..\..\String\String.h"

class StringReader
{
public:
	void operator()(const std::string& name, const std::string& value, String& destination) const;

private:
	static const size_t BUFFER_SIZE = 250;
};

#endif //__STRING_READER_H_INCLUDED__