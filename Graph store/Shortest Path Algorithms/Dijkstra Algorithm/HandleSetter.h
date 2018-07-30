#ifndef __HANDLE_SETTER_H_INCLUDED__
#define __HANDLE_SETTER_H_INCLUDED__

#include "../../Graph/Vertex/Vertex.h"

class HandleSetter
{
public:
	void operator()(Vertex& vertex, const PriorityQueueHandle& handle) const
	{
		vertex.setPriorityQueueHandle(handle);
	}
};

#endif //__HANDLE_SETTER_H_INCLUDED__
