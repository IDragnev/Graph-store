#ifndef __SERIALIZATION_H_INCLUDED__
#define __SERIALIZATION_H_INCLUDED__

#include "Functional\Functional.h"

namespace IDragnev::GraphStore::Serialization
{
	enum class EdgeFormat : char
	{
		edgeStart = '(',
		edgeEnd = ')',
		attributeDelimiter = ',',
		edgeDelimiter = '\n'
	};

	inline const auto correspondingGraphFile = Functional::plus(".txt");
}

#endif //__SERIALIZATION_H_INCLUDED__