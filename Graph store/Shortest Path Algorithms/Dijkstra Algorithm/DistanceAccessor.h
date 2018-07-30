#ifndef __DIST_ACESSOR_H_INCLUDED__
#define __DIST_ACESSOR_H_INCLUDED__

#include "../../Graph/Vertex/Vertex.h"

class DistanceAccessor
{
public:
	void setKeyOf(Vertex& vertex, const Distance& distance) const
	{
		vertex.setDistanceToSource(distance);
	}

	const Distance& getKeyOf(const Vertex& vertex) const
	{
		return vertex.getDistanceToSource();
	}
};

#endif //__DIST_ACESSOR_H_INCLUDED__