#ifndef __PARSE_FAIL_EXCEPTION_H_INCLUDED__
#define __PARSE_FAIL_EXCEPTION_H_INCLUDED__

#include "..\..\General Exceptions\Exception.h"

class String;

class ParseFail : public Exception
{
public:
	ParseFail(const String& filename, const String& reason, unsigned line);

private:
	static std::string buildMessage(const String& filename, const String& reason, unsigned line);
};

#endif //__PARSE_FAIL_EXCEPTION_H_INCLUDED__