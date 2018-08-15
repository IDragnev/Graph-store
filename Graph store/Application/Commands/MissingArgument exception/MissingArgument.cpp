#include "MissingArgument.h"
#include "..\..\..\String\String.h"


MissingArgument::MissingArgument(const String& name) :
	Exception{ "Missing argument: [" + name + "]" }
{
}