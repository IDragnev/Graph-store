#ifndef __CONCRETE_GRAPH_CREATOR__
#define __CONCRETE_GRAPH_CREATOR__

#include "Graph Factory\Graph creators\Base\GraphCreator.h"

namespace IDragnev::GraphStore
{
	template <typename GraphType>
	class ConcreteGraphCreator : public GraphCreator
	{
	public:
		using GraphCreator::GraphCreator;

		GraphPtr operator()(const String& ID) const override
		{
			return std::make_unique<GraphType>(ID);
		}
	};
}

#endif //__CONCRETE_GRAPH_CREATOR__