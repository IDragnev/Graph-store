#ifndef __STRING_READER_H_INCLUDED__
#define __STRING_READER_H_INCLUDED__

#include <string>
#include "..\..\..\..\String\String.h"

class StringReader
{
public:
	void operator()(const std::string& name, const std::string& value, String& destination) const
	{
		destination = value.c_str();
	}
};

#endif //__STRING_READER_H_INCLUDED__