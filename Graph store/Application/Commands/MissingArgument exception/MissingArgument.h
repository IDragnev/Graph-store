#ifndef __MISSING_ARG_H_INCLUDED__
#define __MISSING_ARG_H_INCLUDED__

#include "..\..\..\General Exceptions\Exception.h"
#include <string>

class MissingArgument : public Exception
{
public:
	MissingArgument(const std::string& name);
	virtual ~MissingArgument() override = default;
};

#endif //__MISSING_ARG_H_INCLUDED__