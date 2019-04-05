#ifndef __EDGE_FORMAT_H_INCLUDED__
#define __EDGE_FORMAT_H_INCLUDED__

namespace IDragnev::GraphStore
{
	enum class EdgeFormat : char
	{
		edgeStart = '(',
		edgeEnd = ')',
		attributeDelimiter = ',',
		edgeDelimiter = '\n'
	};
}

#endif //__EDGE_FORMAT_H_INCLUDED__