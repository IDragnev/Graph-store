#ifndef __CONCRETE_GRAPH_CREATOR__
#define __CONCRETE_GRAPH_CREATOR__

#include "..\Base\GraphCreator.h"
#include "..\..\..\Graph\Base Graph\Graph.h"

template <typename CreatedGraph>
class ConcreteGraphCreator : public GraphCreator
{
public:
	using GraphCreator::GraphCreator;
	virtual ~ConcreteGraphCreator() override = default;

	virtual GraphPtr createEmptyGraph(const String& ID) const override
	{
		return GraphPtr{ new CreatedGraph{ ID } };
	}
};

#endif //__CONCRETE_GRAPH_CREATOR__