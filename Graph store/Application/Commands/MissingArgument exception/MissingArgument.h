#ifndef __MISSING_ARG_H_INCLUDED__
#define __MISSING_ARG_H_INCLUDED__

#include "..\..\..\General Exceptions\Exception.h"
#include <string>

class MissingArgument : public Exception
{
public:
	MissingArgument(const std::string& name) : 
		Exception{ "Missing argument: [" + name + "]" } {}
};

#endif //__MISSING_ARG_H_INCLUDED__