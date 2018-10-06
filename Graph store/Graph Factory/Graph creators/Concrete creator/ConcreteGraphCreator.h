#ifndef __CONCRETE_GRAPH_CREATOR__
#define __CONCRETE_GRAPH_CREATOR__

#include "..\Base\GraphCreator.h"
#include "..\..\..\Graph\Base Graph\Graph.h"

template <typename GraphType>
class ConcreteGraphCreator : public GraphCreator
{
public:
	using GraphCreator::GraphCreator;

	GraphPtr createEmptyGraph(const String& ID) const override
	{
		return GraphPtr{ new GraphType{ ID } };
	}
};

#endif //__CONCRETE_GRAPH_CREATOR__